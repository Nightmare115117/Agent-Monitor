#pragma once
#include <string>

class Disk {

    std::string device;
    float usage;
    float used;
    float free;
    float usageP;

public:
    Disk();

    const std::string& getDevice();
    void setDevice(const std::string& device); 

    float getUsage() const;
    void setUsage(float value);

    float getUsed() const;
    void setUsed(float value);

    float getFree() const;
    void setFree(float value);

    float getUsageP() const;
    void setUsageP(float value);

};