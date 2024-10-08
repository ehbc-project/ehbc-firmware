#include <stdio.h>

#include "romfs.h"

int main(int argc, char **argv)
{
    romfs_create("ROMFS TEST");

    romfs_save("asdf.img");

    romfs_cleanup();
    return 0;
}
