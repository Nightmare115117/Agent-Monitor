#include "cpuReader.hpp"

#include <charconv>
#include <cerrno>
#include <fstream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <system_error>
#include <sys/utsname.h>

namespace {

std::string_view trim(std::string_view text) {
	const auto first = text.find_first_not_of(" \t\r\n");
	if (first == std::string_view::npos) {
		return {};
	}
	const auto last = text.find_last_not_of(" \t\r\n");
	return text.substr(first, last - first + 1);
}

std::uint64_t parseCounter(std::string_view value) {
	std::uint64_t result{};
	const auto [end, error] = std::from_chars(value.data(), value.data() + value.size(), result);
	if (value.empty() || error != std::errc{} || end != value.data() + value.size()) {
		throw std::runtime_error("Invalid numeric value in CPU procfs data");
	}
	return result;
}

std::uint64_t checkedAdd(std::uint64_t left, std::uint64_t right) {
	if (right > std::numeric_limits<std::uint64_t>::max() - left) {
		throw std::overflow_error("CPU counter total overflow");
	}
	return left + right;
}

std::string readFile(const char* path) {
	std::ifstream file(path);
	if (!file) {
		throw std::system_error(errno, std::generic_category(), std::string("Unable to open ") + path);
	}

	std::ostringstream contents;
	contents << file.rdbuf();
	if (file.bad()) {
		throw std::runtime_error(std::string("Unable to read ") + path);
	}
	return contents.str();
}

std::optional<std::uint64_t> parseOptionalCounter(std::string_view value) {
	if (value.empty()) {
		return std::nullopt;
	}
	return parseCounter(value);
}

} // namespace

std::uint64_t CPUStatSample::total() const {
	auto result = checkedAdd(user, nice);
	result = checkedAdd(result, system);
	result = checkedAdd(result, idle);
	result = checkedAdd(result, iowait);
	result = checkedAdd(result, irq);
	result = checkedAdd(result, softirq);
	return checkedAdd(result, steal);
}

std::uint64_t CPUStatSample::busy() const {
	auto result = checkedAdd(user, nice);
	result = checkedAdd(result, system);
	result = checkedAdd(result, irq);
	result = checkedAdd(result, softirq);
	return checkedAdd(result, steal);
}

CPUStatSample CPUReader::parseProcStat(std::string_view contents) {
	std::istringstream lines{std::string(contents)};
	std::string line;
	while (std::getline(lines, line)) {
		std::istringstream fields(line);
		std::string label;
		fields >> label;
		if (label != "cpu") {
			continue;
		}

		CPUStatSample sample;
		std::string value;
		std::uint64_t* counters[] = {
			&sample.user, &sample.nice, &sample.system, &sample.idle,
			&sample.iowait, &sample.irq, &sample.softirq, &sample.steal};
		for (auto* counter : counters) {
			if (!(fields >> value)) {
				throw std::runtime_error("Incomplete aggregate CPU line in /proc/stat");
			}
			*counter = parseCounter(value);
		}
		return sample;
	}
	throw std::runtime_error("Aggregate CPU line not found in /proc/stat");
}

CPUInfo CPUReader::parseProcCpuInfo(std::string_view contents) {
	CPUInfo info;
	bool hasModelName = false;
	bool hasVendorId = false;
	bool hasCores = false;
	bool hasSiblings = false;

	std::istringstream lines{std::string(contents)};
	std::string line;
	while (std::getline(lines, line)) {
		const std::string_view entry(line);
		const auto separator = entry.find(':');
		if (separator == std::string_view::npos) {
			continue;
		}

		const auto key = trim(entry.substr(0, separator));
		const auto value = trim(entry.substr(separator + 1));
		if (key == "model name" && !hasModelName) {
			info.modelName = value;
			hasModelName = true;
		} else if (key == "vendor_id" && !hasVendorId) {
			info.vendorId = value;
			hasVendorId = true;
		} else if (key == "cpu cores" && !hasCores) {
			info.coresPerSocket = parseOptionalCounter(value);
			hasCores = true;
		} else if (key == "siblings" && !hasSiblings) {
			info.siblingsPerSocket = parseOptionalCounter(value);
			hasSiblings = true;
		}
	}
	return info;
}

CPUInfo CPUReader::readStaticInfo() {
	auto info = parseProcCpuInfo(readFile("/proc/cpuinfo"));

	struct utsname systemInfo {};
	if (uname(&systemInfo) != 0) {
		throw std::system_error(errno, std::generic_category(), "Unable to determine CPU architecture");
	}
	info.architecture = systemInfo.machine;
	return info;
}

CPU CPUReader::readCPU() {
	return CPU(readStaticInfo(), readMetrics());
}

std::optional<double> CPUReader::calculateUsage(
	const CPUStatSample& previous,
	const CPUStatSample& current) {
	const auto previousTotal = previous.total();
	const auto currentTotal = current.total();
	const auto previousBusy = previous.busy();
	const auto currentBusy = current.busy();

	if (currentTotal < previousTotal || currentBusy < previousBusy) {
		return std::nullopt;
	}

	const auto deltaTotal = currentTotal - previousTotal;
	if (deltaTotal == 0) {
		return std::nullopt;
	}
	const auto deltaBusy = currentBusy - previousBusy;
	return static_cast<double>(deltaBusy) / static_cast<double>(deltaTotal) * 100.0;
}

CPUMetrics CPUReader::readMetrics() {
	const auto current = parseProcStat(readFile("/proc/stat"));
	CPUMetrics metrics;
	std::lock_guard lock(sampleMutex_);
	if (previousSample_) {
		metrics.usagePercent = calculateUsage(*previousSample_, current);
	}
	previousSample_ = current;
	return metrics;
}
