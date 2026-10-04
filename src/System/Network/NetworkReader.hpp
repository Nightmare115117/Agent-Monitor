#pragma once

#include "Network.hpp"
#include <string>
#include <vector>


class NetworkReader {
    
    static std::vector<std::string> section(const std::string& linea);
    static std::vector<std::string> file();
    static float bytesToMegabytes(float bytes);

public: 
    static std::vector<Network> reader();

};