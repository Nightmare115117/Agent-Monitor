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
