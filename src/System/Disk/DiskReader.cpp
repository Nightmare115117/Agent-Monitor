#include "DiskReader.hpp"
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

struct statvfs DiskReader::datos;

Disk DiskReader::reader() {

    ifstream archivo("/proc/mounts");

    if (!archivo)
        throw runtime_error("No se pudo abrir el archivo");

    string linea;
    vector<string> procesados;
    vector<Disk> almacenados;
    vector<string> ignorados = {
    "tmpfs",
    "proc",
    "sysfs",
    "overlay"
    };

    while(getline(archivo, linea)) {

        stringstream dato(linea);
        string device;
        string mount;
        string filesys;

        dato >> device >> mount >> filesys;

            if (mount == "/mnt/nas")
                continue;

            if (find(ignorados.begin(), ignorados.end(), filesys) != ignorados.end())
                continue;
            
            if (find(procesados.begin(), procesados.end(), device) != procesados.end())
                continue;

        procesados.push_back(device);
        
        int result = statvfs(mount.c_str(), &datos);

        if (result != 0) 
            throw runtime_error("No se pudo obtener los datos del punto de montado");

        Disk disco;
        disco.setDevice(device);
        disco.setUsage(datos.f_blocks * datos.f_frsize);
        disco.setFree(datos.f_bfree * datos.f_bavail);
        disco.setUsed(disco.getUsage() - disco.getFree());
        disco.setUsageP((disco.getUsed()/disco.getUsage())*100);
        almacenados.push_back(disco);
    }

    
}

void DiskReader::update(Disk& disco) {
    disco = reader();
}