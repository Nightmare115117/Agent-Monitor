#include "cpu.hpp"

#include <utility>

CPU::CPU(CPUInfo info, CPUMetrics metrics)
	: info_(std::move(info)), metrics_(std::move(metrics)) {}

const CPUInfo& CPU::getInfo() const noexcept {
	return info_;
}

const CPUMetrics& CPU::getMetrics() const noexcept {
	return metrics_;
}
