#include <libehbcfw/elf.h>

#include <libehbcfw/asm/elf.h>

static int verify_ident(const union elf_hdr *hdr)
{
    // e_ident is common to elf32 and elf64

    // check magic number
    if ((hdr->elf32.e_ident[EI_MAG0] != ELFMAG0) ||
        (hdr->elf32.e_ident[EI_MAG1] != ELFMAG1) ||
        (hdr->elf32.e_ident[EI_MAG2] != ELFMAG2) ||
        (hdr->elf32.e_ident[EI_MAG3] != ELFMAG3)) {
        return 1;
    }

    // check elf class
    if (hdr->elf32.e_ident[EI_CLASS] == ELFCLASSNONE) {
        return 1;
    }

    // check data order
    if (hdr->elf32.e_ident[EI_DATA] == ELFDATANONE) {
        return 1;
    }

    // check version
    if (hdr->elf32.e_ident[EI_VERSION] != EV_CURRENT) {
        return 1;
    }

    // skip EI_OSABI, EI_ABIVERSION
    return 0;
}

const uint8_t *elf_get_ident(struct elf_file *elf)
{
    return elf->header.elf32.e_ident;
}
