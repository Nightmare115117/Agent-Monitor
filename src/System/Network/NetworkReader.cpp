#include "NetworkReader.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <regex>

using namespace std;

float NetworkReader::bytesToMegabytes(float bytes) {
    return bytes / (1024.0f * 1024.0f);
}

vector<Network> NetworkReader::reader() {
    vector<string> datos = file();
    vector<Network> networks;

    for (size_t index = 0; index + 4 < datos.size(); index += 5) {
        Network network;
        network.setInterfaceName(datos[index]);
        network.setReceivedBytes(bytesToMegabytes(stof(datos[index + 1])));
        network.setReceivedPackets(stoull(datos[index + 2]));
        network.setTransmittedBytes(bytesToMegabytes(stof(datos[index + 3])));
        network.setTransmittedPackets(stoull(datos[index + 4]));
        networks.push_back(network);
    }

    return networks;
}

vector<string> NetworkReader::section(const string& linea) {
    istringstream stream(linea);
    string interface;
    if (!(stream >> interface) || interface.back() != ':')
        return {};

    interface.pop_back();
    vector<string> counters;
    string counter;
    while (stream >> counter)
        counters.push_back(counter);

    if (counters.size() < 16)
        return {};

    return {interface, counters[0], counters[1], counters[8], counters[9]};
}

vector<string> NetworkReader::file() {
    ifstream archivo("/proc/net/dev");

    if (!archivo)
        throw runtime_error("No se encontro el archivo");

    string line;
    vector<string> datos;

    while (getline(archivo, line)) {
        vector<string> values = section(line);
        if (values.empty())
            continue;

        const string& interface = values[0];
        if ((regex_match(interface, regex("enp[0-9]+s[0-9]+"))) || 
        (regex_match(interface, regex("flannel\\.[0-9]+"))) || 
        (regex_match(interface, regex("cni[0-9]+"))))
            datos.insert(datos.end(), values.begin(), values.end());
    }

    return datos;
}