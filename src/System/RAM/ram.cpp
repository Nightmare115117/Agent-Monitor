#include "ram.hpp"

RAM::RAM() : total(0.0f), cache(0.0f), available(0.0f), swap_total(0.0f), swap_used(0.0f) {}

float RAM::getTotal() const {
    return total;
}

void RAM::setTotal(float value) {
    total = value;
}

float RAM::getCache() const {
    return cache;
}

void RAM::setCache(float value) {
    cache = value;
}

float RAM::getAvailable() const {
    return available;
}

void RAM::setAvailable(float value) {
    available = value;
}

float RAM::getSwapTotal() const {
    return swap_total;
}

void RAM::setSwapTotal(float value) {
    swap_total = value;
}

float RAM::getSwapUsed() const {
    return swap_used;
}

void RAM::setSwapUsed(float value) {
    swap_used = value;
}
