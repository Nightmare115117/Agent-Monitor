#include "ramReader.hpp"
#include <fstream>
#include <string>
#include <vector>

using namespace std;

const static float convert = 1024;

RAM RAMReader::reader() {
    ifstream archivo("/proc/meminfo");

    if (!archivo)
        throw runtime_error("No se encontro el archivo");

    string dato;
    int count = 0;
    vector<long> datos;

    do {
        getline(archivo, dato);

        if (archivo.fail())
            break;
        
        if (dato.substr(0, dato.find(":")) == "MemTotal") {
            datos.push_back(section(dato));
            count++;
        }

        if (dato.substr(0, dato.find(":")) == "MemAvailable") {
            datos.push_back(section(dato));
            count++;
        }

       if (dato.substr(0, dato.find(":")) == "Cached") {
            datos.push_back(section(dato));
            count++;
        }

       if (dato.substr(0, dato.find(":")) == "SwapTotal") {
            datos.push_back(section(dato));
            count++;
        }

        if (dato.substr(0, dato.find(":")) == "SwapFree") {
            datos.push_back(section(dato));
            count++;
        }

        if (count == 5)
            break;
    } while(!archivo.eof());

    if (datos.size() < 5)
        throw runtime_error("No se pudieron recolectar los datos necesarios");

    RAM ram = RAM();
    ram.setTotal(datos[0]/convert/convert);
    ram.setAvailable(datos[1]/convert/convert);
    ram.setCache(datos[2]/convert/convert);
    ram.setSwapTotal(datos[3]/convert/convert);
    ram.setSwapUsed((datos[3] - datos[4])/convert/convert);

    return ram;
}

long RAMReader::section(const string& linea) {
    int pos = linea.find(":");
    int num = linea.find_first_not_of(" ", pos+1);
    return stol(linea.substr(num, linea.substr(num).length()-3));
}