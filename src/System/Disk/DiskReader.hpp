#pragma once

#include "Disk.hpp"
#include <sys/statvfs.h>


class DiskReader {

static struct statvfs datos;

public:

    static Disk reader();
    static void update(Disk& disco);

};