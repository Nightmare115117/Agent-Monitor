#pragma once

#include <string>

class Network {

    std::string interfaceName;
    float receivedBytes;
    float transmittedBytes;
    u_int64_t receivedPackets;
    u_int64_t transmittedPackets;

public: 
    Network();

    const std::string& getInterfaceName() const;
    void setInterfaceName(const std::string& value);

    float getReceivedBytes() const;
    void setReceivedBytes(float value);

    float getTransmittedBytes() const;
    void setTransmittedBytes(float value);

    u_int64_t getReceivedPackets() const;
    void setReceivedPackets(u_int64_t value);

    u_int64_t getTransmittedPackets() const;
    void setTransmittedPackets(u_int64_t value);

};