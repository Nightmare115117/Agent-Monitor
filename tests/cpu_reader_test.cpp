#include <gtest/gtest.h>
#include "System/CPU/cpuReader.hpp"

TEST(CPUReaderTest, ParsesOnlyTheAggregateCpuLine) {
	const auto sample = CPUReader::parseProcStat(
		"cpu  100 20 30 400 50 6 7 8 900 1000\n"
		"cpu0 9000 0 0 0 0 0 0 0\n"
		"cpu1 0 9000 0 0 0 0 0 0\n");

	EXPECT_EQ(sample.user, 100);
	EXPECT_EQ(sample.nice, 20);
	EXPECT_EQ(sample.system, 30);
	EXPECT_EQ(sample.idle, 400);
	EXPECT_EQ(sample.iowait, 50);
	EXPECT_EQ(sample.irq, 6);
	EXPECT_EQ(sample.softirq, 7);
	EXPECT_EQ(sample.steal, 8);
	EXPECT_EQ(sample.total(), 621);
	EXPECT_EQ(sample.busy(), 171);
}

TEST(CPUReaderTest, RejectsMissingOrIncompleteAggregateCpuLine) {
	EXPECT_THROW(CPUReader::parseProcStat("cpu0 1 2 3 4 5 6 7 8\n"), std::runtime_error);
	EXPECT_THROW(CPUReader::parseProcStat("cpu 1 2 3 4\n"), std::runtime_error);
}

TEST(CPUReaderTest, ParsesCpuInfoWithoutConfusingCoresAndSiblings) {
	const auto info = CPUReader::parseProcCpuInfo(
		"processor : 0\n"
		"vendor_id : GenuineVendor\n"
		"model name : Example CPU\n"
		"cpu cores : 6\n"
		"siblings : 12\n\n"
		"processor : 1\n"
		"model name : Different later entry\n");

	EXPECT_EQ(info.vendorId, "GenuineVendor");
	EXPECT_EQ(info.modelName, "Example CPU");
	ASSERT_TRUE(info.coresPerSocket.has_value());
	ASSERT_TRUE(info.siblingsPerSocket.has_value());
	EXPECT_EQ(*info.coresPerSocket, 6);
	EXPECT_EQ(*info.siblingsPerSocket, 12);
}

TEST(CPUReaderTest, CalculatesBusyUsageExcludingIoWait) {
	const CPUStatSample previous{100, 10, 20, 300, 40, 5, 6, 7};
	const CPUStatSample current{104, 10, 20, 303, 43, 5, 6, 7};

	const auto usage = CPUReader::calculateUsage(previous, current);

	ASSERT_TRUE(usage.has_value());
	EXPECT_DOUBLE_EQ(*usage, 40.0);
}

TEST(CPUReaderTest, ReturnsNoUsageForZeroDeltaOrResetCounters) {
	const CPUStatSample sample{100, 10, 20, 300, 40, 5, 6, 7};
	const CPUStatSample reset{1, 1, 1, 1, 1, 1, 1, 1};

	EXPECT_FALSE(CPUReader::calculateUsage(sample, sample).has_value());
	EXPECT_FALSE(CPUReader::calculateUsage(sample, reset).has_value());
}

