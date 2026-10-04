#pragma once

#include <cstdint>
#include <optional>
#include <string>

struct CPUInfo {
	std::string modelName;
	std::string vendorId;
	std::optional<std::uint64_t> coresPerSocket;
	std::optional<std::uint64_t> siblingsPerSocket;
	std::string architecture;
};

struct CPUMetrics {
	std::optional<double> usagePercent;
};

class CPU {
public:
	CPU() = default;
	CPU(CPUInfo info, CPUMetrics metrics);

	const CPUInfo& getInfo() const noexcept;
	const CPUMetrics& getMetrics() const noexcept;

private:
	CPUInfo info_;
	CPUMetrics metrics_;
};
