#include <gtest/gtest.h>
#include "System/Network/NetworkReader.hpp"

using namespace std;

TEST(NetworkReader, ReaderDontThrowException) {
EXPECT_NO_THROW(NetworkReader::reader());
}