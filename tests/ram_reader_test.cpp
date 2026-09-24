#include <gtest/gtest.h>
#include "System/RAM/ramReader.hpp"

TEST(RAMReader, ReaderDontThrowException) {
    EXPECT_NO_THROW(RAMReader::reader);
}

TEST(RAMReader, TotalmayorThanCero) {
    RAM ram = RAMReader::reader();
    EXPECT_GT(ram.getTotal(), 0);
}