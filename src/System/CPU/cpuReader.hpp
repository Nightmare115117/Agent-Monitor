#pragma once

#include "cpu.hpp"

#include <cstdint>
#include <mutex>
#include <optional>
#include <string_view>

struct CPUStatSample {
	std::uint64_t user{};
	std::uint64_t nice{};
	std::uint64_t system{};
	std::uint64_t idle{};
	std::uint64_t iowait{};
	std::uint64_t irq{};
	std::uint64_t softirq{};
	std::uint64_t steal{};

	std::uint64_t total() const;
	std::uint64_t busy() const;
};

class CPUReader {
public:
	CPUReader() = delete;

	static CPU readCPU();
	static CPUInfo readStaticInfo();
	static CPUMetrics readMetrics();

	static CPUStatSample parseProcStat(std::string_view contents);
	static CPUInfo parseProcCpuInfo(std::string_view contents);
	static std::optional<double> calculateUsage(
		const CPUStatSample& previous,
		const CPUStatSample& current);

private:
	inline static std::mutex sampleMutex_;
	inline static std::optional<CPUStatSample> previousSample_;
};
