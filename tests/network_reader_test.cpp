#include <gtest/gtest.h>
#include "System/Network/NetworkReader.hpp"

using namespace std;

TEST(NetworkReader, ConvertsBytesToMegabytes) {
    EXPECT_FLOAT_EQ(NetworkReader::bytesToMegabytes(1048576.0f), 1.0f);
    EXPECT_FLOAT_EQ(NetworkReader::bytesToMegabytes(524288.0f), 0.5f);
}

TEST(NetworkReader, ReaderDontThrowException) {
EXPECT_NO_THROW(NetworkReader::reader());
}