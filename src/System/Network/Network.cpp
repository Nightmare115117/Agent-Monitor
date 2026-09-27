#include "Network.hpp"

Network::Network()
	: interfaceName(), receivedBytes(0.0f), transmittedBytes(0.0f),
	  receivedPackets(0), transmittedPackets(0) {}

const std::string& Network::getInterfaceName() const {
	return interfaceName;
}

void Network::setInterfaceName(const std::string& value) {
	interfaceName = value;
}

float Network::getReceivedBytes() const {
	return receivedBytes;
}

void Network::setReceivedBytes(float value) {
	receivedBytes = value;
}

float Network::getTransmittedBytes() const {
	return transmittedBytes;
}

void Network::setTransmittedBytes(float value) {
	transmittedBytes = value;
}

u_int64_t Network::getReceivedPackets() const {
	return receivedPackets;
}

void Network::setReceivedPackets(u_int64_t value) {
	receivedPackets = value;
}

u_int64_t Network::getTransmittedPackets() const {
	return transmittedPackets;
}

void Network::setTransmittedPackets(u_int64_t value) {
	transmittedPackets = value;
}
