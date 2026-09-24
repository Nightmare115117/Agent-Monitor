#pragma once
#include "ram.hpp"
#include <string>

class RAMReader {
    
    static long section(const std::string& linea);

public: 

    static RAM reader();
    static void updater();

};