#pragma once

#include "Network.hpp"
#include <string>
#include <vector>


class NetworkReader {
    
    static std::vector<std::string> section(const std::string& linea);
    static std::vector<std::string> file();

public: 

    static std::vector<Network> reader();
    static void updater(std::vector<Network>& read);

};