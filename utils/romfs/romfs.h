#ifndef __ROMFS_H__
#define __ROMFS_H__

#include <stdint.h>

struct romfs_fshdr {
    char magic[4];                  // ASCII '\0', 'r', 'o', 'm'
    uint32_t version;               // fs version
    uint32_t fs_size;               // entire filesystem size
    uint32_t fshdr_size;            // size of the filesystem header excluding the string table
    uint32_t first_filehdr_offs;    // offset of the first file
    uint32_t vname_offs;            // offset of volume name from string table
    uint32_t checksum;              // checksum of fs header and string table
    char strtab[];
};

struct romfs_filehdr {
    uint32_t next_filehdr_offs;     // offset of next file header, zero if end of the list
    uint32_t flags;                 // file flags
    uint32_t fname_offs;            // offset of the file name from string table
    uint32_t data_len;              // data size
    uint32_t checksum;              // checksum of the header and data
    uint8_t data[];                 // file data. if the directory flag is set, it contains offset of the head of the file header list (four bytes)
};

#endif // __ROMFS_H__
