#include "romfs.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define ALIGN16(val) (((val) + 15) & ~0xF)

extern int romfs_read(size_t offset, void *buf, size_t len);
extern int romfs_write(size_t offset, const void *buf, size_t len);

static struct romfs_fshdr fs_header;

static char *strtab = NULL;
static size_t strtab_size = 0;
static struct romfs_filehdr *files = NULL;
static size_t files_size = 0;

static uint32_t strtab_add(const char *str)
{
    size_t str_size = strlen(str) + 1;
    uint32_t str_offs = strtab_size;

    strtab_size += str_size;
    if (!strtab) {
        strtab = malloc(strtab_size);
    } else {
        strtab = realloc(strtab, strtab_size);
    }

    strcpy(strtab + str_offs, str);

    return str_offs;
}

void romfs_create(const char *vol_name)
{
    fs_header.magic[0] = '\0';
    fs_header.magic[1] = 'r';
    fs_header.magic[2] = 'o';
    fs_header.magic[3] = 'm';
    fs_header.version = 0;

    fs_header.fs_size = 0;
    fs_header.fshdr_size = sizeof(struct romfs_fshdr);

    fs_header.first_filehdr_offs = 0;

    fs_header.checksum = 0;

    if (strtab) free(strtab);
    strtab = NULL;
    strtab_size = 0;

    if (files) free(files);
    files = NULL;
    files_size = 0;

    fs_header.vname_offs = strtab_add(vol_name);
}

int romfs_mkdir(const char *dir_name)
{
    return 0;
}

int romfs_mkfile(const char *file_name, const void *buf, size_t len)
{
    uint32_t file_offs = files_size;
    struct romfs_filehdr *ptr = (struct romfs_filehdr*)((uint8_t*)files + file_offs);

    files_size += ALIGN16(sizeof(struct romfs_filehdr) + len);
    if (!files) {
        files = malloc(files_size);
    } else {
        files = realloc(files, files_size);
    }

    ptr->next_filehdr_offs = 0;
    ptr->flags = 0;
    ptr->fname_offs = strtab_add(file_name);
    ptr->checksum = 0;
    ptr->data_len = len;
    memcpy(ptr->data, buf, len);

    return 0;
}

void romfs_cleanup(void)
{
    free(strtab);
    strtab = NULL;
    free(files);
    files = NULL;
}
