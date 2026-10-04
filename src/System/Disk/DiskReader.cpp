#include "DiskReader.hpp"

#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/sysmacros.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace fs = std::filesystem;
using namespace std;

struct statvfs DiskReader::datos;

bool DiskReader::isIgnoredFilesystem(const string& filesystem) {
    static const set<string> ignored = {
        "tmpfs", "proc", "sysfs", "overlay", "squashfs", "devtmpfs",
        "debugfs", "securityfs", "fusectl", "cgroup", "cgroup2",
        "mqueue", "ramfs", "autofs", "hugetlbfs", "pstore"
    };
    return ignored.count(filesystem) > 0;
}

// Devuelve el nombre del dispositivo de bloque (sda1, nvme0n1p2, dm-0...)
// a partir de la línea de /proc/mounts. Vacío si no es un dispositivo real.
string DiskReader::normalizeDeviceName(const string& source,
                                       const string& mountPoint) {
    if (source.rfind("/dev/", 0) != 0)
        return {};

    std::error_code ec;

    // 1) Lo más fiable: major:minor del punto de montaje -> sysfs.
    //    Funciona con /dev/root, /dev/mapper/*, by-uuid, etc.
    struct stat st{};
    if (stat(mountPoint.c_str(), &st) == 0 && major(st.st_dev) != 0) {
        fs::path p = "/sys/dev/block/" + to_string(major(st.st_dev)) + ":" +
                     to_string(minor(st.st_dev));
        auto resolved = fs::canonical(p, ec);
        if (!ec)
            return resolved.filename().string();
    }

    // 2) Fallback: resolver el symlink del dispositivo (btrfs, zfs, etc.).
    ec.clear();
    auto resolved = fs::canonical(fs::path(source), ec);
    if (!ec)
        return resolved.filename().string();

    return {};
}

// Resuelve un dispositivo de bloque a sus discos físicos:
//  - dm-*/md* (LVM, LUKS, RAID): sigue /slaves recursivamente
//  - particiones: sube al directorio padre en sysfs (funciona con
//    sda1, nvme0n1p3, mmcblk0p1, vda1, xvda1... sin adivinar nombres)
//  - disco entero: se devuelve tal cual
set<string> DiskReader::physicalDisks(const string& deviceName, int depth) {
    set<string> result;
    if (deviceName.empty() || depth > 8)
        return result;

    std::error_code ec;
    const fs::path sysPath =
        fs::canonical(fs::path("/sys/class/block") / deviceName, ec);
    if (ec)
        return { deviceName };

    // Dispositivos apilados (device-mapper, md)
    const fs::path slavesPath = sysPath / "slaves";
    if (fs::is_directory(slavesPath, ec)) {
        for (const auto& slave : fs::directory_iterator(slavesPath, ec)) {
            auto sub = physicalDisks(slave.path().filename().string(), depth + 1);
            result.insert(sub.begin(), sub.end());
        }
        if (!result.empty())
            return result;
    }

    // Partición: el padre en sysfs es el disco
    ec.clear();
    if (fs::exists(sysPath / "partition", ec)) {
        string parent = sysPath.parent_path().filename().string();
        if (!parent.empty())
            return { parent };
    }

    return { deviceName };
}

Disk DiskReader::buildDisk(const string& name, const string& mountPoint) {
    struct statvfs st{};
    if (statvfs(mountPoint.c_str(), &st) != 0)
        throw runtime_error("statvfs fallo");

    const double total = static_cast<double>(st.f_blocks) * st.f_frsize;
    const double free_ = static_cast<double>(st.f_bavail) * st.f_frsize;
    const double used  = total - free_;

    Disk d;
    d.setDevice(name);
    d.setUsage(static_cast<float>(total));   // ver nota sobre float
    d.setFree(static_cast<float>(free_));
    d.setUsed(static_cast<float>(used));
    d.setUsageP(total > 0.0 ? static_cast<float>(used / total * 100.0) : 0.0f);
    return d;
}

vector<Disk> DiskReader::readAll(string* rootDisk) {
    ifstream archivo("/proc/mounts");
    if (!archivo)
        throw runtime_error("No se pudo abrir /proc/mounts");

    map<string, Disk> porDisco;
    string linea;

    while (getline(archivo, linea)) {
        stringstream stream(linea);
        string device, mountPoint, filesystem;
        if (!(stream >> device >> mountPoint >> filesystem))
            continue;
        if (isIgnoredFilesystem(filesystem))
            continue;

        const string block = normalizeDeviceName(device, mountPoint);
        if (block.empty() || block.rfind("loop", 0) == 0)
            continue;

        const set<string> disks = physicalDisks(block);
        if (disks.empty())
            continue;

        const string group = *disks.begin();   // con RAID: primer disco (documentado)
        if (porDisco.count(group) && mountPoint != "/")
            continue;                          // primer montaje por disco, salvo "/"

        try {
            porDisco[group] = buildDisk(group, mountPoint);
        } catch (const exception&) {
            continue;
        }

        if (mountPoint == "/" && rootDisk)
            *rootDisk = group;
    }

    if (porDisco.empty())
        throw runtime_error("No se encontraron discos montados");

    vector<Disk> out;
    for (auto& kv : porDisco)
        out.push_back(kv.second);
    return out;
}

Disk DiskReader::reader() {
    string rootDisk;
    vector<Disk> all = readAll(&rootDisk);

    if (!rootDisk.empty()) {
        for (const auto& d : all)
            if (d.getDevice() == rootDisk)
                return d;
    }
    return all.front();
}