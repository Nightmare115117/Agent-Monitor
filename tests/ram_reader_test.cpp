#include <gtest/gtest.h>
#include "System/RAM/RamReader.hpp"

using namespace std;

TEST(RAMReader, ReaderDontThrowException) {
    EXPECT_NO_THROW(RAMReader::reader());
}

TEST(RAMReader, TotalmayorThanCero) {
    RAM ram = RAMReader::reader();
    EXPECT_GT(ram.getTotal(), 0);
}

TEST(RAMReader, AvailableLTTotal) {
    RAM ram = RAMReader::reader();
    EXPECT_LT(ram.getAvailable(), ram.getTotal());
}

TEST(RAMReader, CacheLTTotal) {
    RAM ram = RAMReader::reader();
    EXPECT_LT(ram.getCache(), ram.getTotal());
}

TEST(RAMReader, SWAPGE0) {
    RAM ram = RAMReader::reader();
    EXPECT_GE(ram.getSwapTotal(), 0);
}

TEST(RAMReader, SWAPLETotal) {
    RAM ram = RAMReader::reader();
    EXPECT_LE(ram.getSwapUsed(), ram.getSwapTotal());
}