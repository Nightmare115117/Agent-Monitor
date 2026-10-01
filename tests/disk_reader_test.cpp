#include <gtest/gtest.h>
#include "System/Disk/DiskReader.hpp"

TEST(DiskReaderTest, ReturnsAValidDisk) {
    EXPECT_NO_THROW({
        Disk disk = DiskReader::reader();
        EXPECT_FALSE(disk.getDevice().empty());
        EXPECT_GE(disk.getUsage(), 0.0f);
    });
}
