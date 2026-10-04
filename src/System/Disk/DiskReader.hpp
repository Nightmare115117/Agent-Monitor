#include <set>
#include <string>
#include <vector>
#include <sys/statvfs.h>
#include "Disk.hpp"

class DiskReader {
public:
    static Disk reader();                       // disco de "/"
    static std::vector<Disk> readAll(std::string* rootDisk = nullptr);

private:
    static struct statvfs datos;
    static bool isIgnoredFilesystem(const std::string& filesystem);
    static std::string normalizeDeviceName(const std::string& source,
                                           const std::string& mountPoint);
    static std::set<std::string> physicalDisks(const std::string& deviceName,
                                               int depth = 0);
    static Disk buildDisk(const std::string& name, const std::string& mountPoint);
};