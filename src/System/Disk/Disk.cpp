#include "Disk.hpp"

using namespace std;

Disk::Disk() : device(""), usage(0.0f), used(0.0f), free(0.0f), usageP(0.0f) {}

const string& Disk::getDevice() {
	return device;
}

void Disk::setDevice(const string& device) {
	this->device = device;
}

float Disk::getUsage() const {
	return usage;
}

void Disk::setUsage(float value) {
	usage = value;
}

float Disk::getUsed() const {
	return used;
}

void Disk::setUsed(float value) {
	used = value;
}

float Disk::getFree() const {
	return free;
}

void Disk::setFree(float value) {
	free = value;
}

float Disk::getUsageP() const {
	return usageP;
}

void Disk::setUsageP(float value) {
	usageP = value;
}
