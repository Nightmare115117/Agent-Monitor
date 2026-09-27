#pragma once
#include "ram.hpp"
#include <string>
#include <vector>

class RAMReader {
    
    static long section(const std::string& linea);
    static std::vector<long> file();

public: 

    static RAM reader();
    static void updater(RAM& ram);

};