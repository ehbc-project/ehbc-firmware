#ifndef __LIBEHBCFW_ELF_H__
#define __LIBEHBCFW_ELF_H__

#include <stdint.h>
#include <stddef.h>

// elf header informations are taken from:
// https://refspecs.linuxfoundation.org/elf/gabi4+/ch4.eheader.html

#define EI_MAG0         0
#define EI_MAG1         1
#define EI_MAG2         2
#define EI_MAG3         3
#define EI_CLASS        4
#define EI_DATA         5
#define EI_VERSION      6
#define EI_OSABI        7
#define EI_ABIVERSION   8
#define EI_PAD          9
#define EI_NIDENT       16

#define ELFMAG0         0x7F
#define ELFMAG1         'E'
#define ELFMAG2         'L'
#define ELFMAG3         'F'

#define ELFCLASSNONE    0
#define ELFCLASS32      1
#define ELFCLASS64      2

#define ELFDATANONE     0
#define ELFDATA2LSB     1  // little-endian 
#define ELFDATA2MSB     2  // big-endian

#define ELFOSABI_NONE   0

#define ET_NONE         0x00
#define ET_REL          0x01
#define ET_EXEC         0x02
#define ET_DYN          0x03
#define ET_CORE         0x04
#define ET_LOOS         0xFE00
#define ET_HIOS         0xFEFF
#define ET_LOPROC       0xFF00
#define ET_HIPROC       0xFFFF

#define EM_NONE         0   // No machine
#define EM_M32          1   // AT&T WE 32100
#define EM_SPARC        2   // SPARC
#define EM_386          3   // Intel 80386
#define EM_68K          4   // Motorola 68000
#define EM_88K          5   // Motorola 88000
#define EM_860          7   // Intel 80860
#define EM_MIPS         8   // MIPS I Architecture
#define EM_S370         9   // IBM System/370 Processor
#define EM_MIPS_RS3_LE	10  // MIPS RS3000 Little-endian
#define EM_PARISC       15  // Hewlett-Packard PA-RISC
#define EM_VPP500       17  // Fujitsu VPP500
#define EM_SPARC32PLUS  18  // Enhanced instruction set SPARC
#define EM_960          19  // Intel 80960
#define EM_PPC          20  // PowerPC
#define EM_PPC64        21  // 64-bit PowerPC
#define EM_S390         22  // IBM System/390 Processor
#define EM_V800         36  // NEC V800
#define EM_FR20         37  // Fujitsu FR20
#define EM_RH32         38  // TRW RH-32
#define EM_RCE          39  // Motorola RCE
#define EM_ARM          40  // Advanced RISC Machines ARM
#define EM_ALPHA        41  // Digital Alpha
#define EM_SH           42  // Hitachi SH
#define EM_SPARCV9      43  // SPARC Version 9
#define EM_TRICOR       44  // Siemens TriCore embedded processor
#define EM_ARC          45  // Argonaut RISC Core, Argonaut Technologies Inc.
#define EM_H8_300       46  // Hitachi H8/300
#define EM_H8_300H      47  // Hitachi H8/300H
#define EM_H8S          48  // Hitachi H8S
#define EM_H8_500       49  // Hitachi H8/500
#define EM_IA_64        50  // Intel IA-64 processor architecture
#define EM_MIPS_X       51  // Stanford MIPS-X
#define EM_COLDFIRE     52  // Motorola ColdFire
#define EM_68HC12       53  // Motorola M68HC12
#define EM_MMA          54  // Fujitsu MMA Multimedia Accelerator
#define EM_PCP          55  // Siemens PCP
#define EM_NCPU         56  // Sony nCPU embedded RISC processor
#define EM_NDR1         57  // Denso NDR1 microprocessor
#define EM_STARCORE     58  // Motorola Star*Core processor
#define EM_ME16         59  // Toyota ME16 processor
#define EM_ST100        60  // STMicroelectronics ST100 processor
#define EM_TINYJ        61  // Advanced Logic Corp. TinyJ embedded processor family
#define EM_X86_64       62  // AMD x86-64 architecture
#define EM_PDSP         63  // Sony DSP Processor
#define EM_PDP10        64  // Digital Equipment Corp. PDP-10
#define EM_PDP11        65  // Digital Equipment Corp. PDP-11
#define EM_FX66         66  // Siemens FX66 microcontroller
#define EM_ST9PLUS      67  // STMicroelectronics ST9+ 8/16 bit microcontroller
#define EM_ST7          68  // STMicroelectronics ST7 8-bit microcontroller
#define EM_68HC16       69  // Motorola MC68HC16 Microcontroller
#define EM_68HC11       70  // Motorola MC68HC11 Microcontroller
#define EM_68HC08       71  // Motorola MC68HC08 Microcontroller
#define EM_68HC05       72  // Motorola MC68HC05 Microcontroller
#define EM_SVX          73  // Silicon Graphics SVx
#define EM_ST19         74  // STMicroelectronics ST19 8-bit microcontroller
#define EM_VAX          75  // Digital VAX
#define EM_CRIS         76  // Axis Communications 32-bit embedded processor
#define EM_JAVELIN      77  // Infineon Technologies 32-bit embedded processor
#define EM_FIREPATH     78  // Element 14 64-bit DSP Processor
#define EM_ZSP          79  // LSI Logic 16-bit DSP Processor
#define EM_MMIX         80  // Donald Knuth's educational 64-bit processor
#define EM_HUANY        81  // Harvard University machine-independent object files
#define EM_PRISM        82  // SiTera Prism
#define EM_AVR          83  // Atmel AVR 8-bit microcontroller
#define EM_FR30         84  // Fujitsu FR30
#define EM_D10V         85  // Mitsubishi D10V
#define EM_D30V         86  // Mitsubishi D30V
#define EM_V850         87  // NEC v850
#define EM_M32R         88  // Mitsubishi M32R
#define EM_MN10300      89  // Matsushita MN10300
#define EM_MN10200      90  // Matsushita MN10200
#define EM_PJ           91  // picoJava
#define EM_OPENRISC     92  // OpenRISC 32-bit embedded processor
#define EM_ARC_A5       93  // ARC Cores Tangent-A5
#define EM_XTENSA       94  // Tensilica Xtensa Architecture
#define EM_VIDEOCORE    95  // Alphamosaic VideoCore processor
#define EM_TMM_GPP      96  // Thompson Multimedia General Purpose Processor
#define EM_NS32K        97  // National Semiconductor 32000 series
#define EM_TPC          98  // Tenor Network TPC processor
#define EM_SNP1K        99  // Trebia SNP 1000 processor
#define EM_ST200        100 // STMicroelectronics (www.st.com) ST200 microcontroller

#define EV_NONE         0
#define EV_CURRENT      1

struct elf32_hdr {
    uint8_t e_ident[EI_NIDENT];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint32_t e_entry;
    uint32_t e_phoff;
    uint32_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
} __attribute__((packed));

struct elf64_hdr {
    uint8_t e_ident[EI_NIDENT];
    uint8_t e_type;
    uint16_t e_version;
    uint64_t e_entry;
    uint64_t e_phoff;
    uint64_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
} __attribute__((packed));

union elf_hdr {
    struct elf32_hdr elf32;
    struct elf64_hdr elf64;
};

#define PT_NULL         0x00000000
#define PT_LOAD         0x00000001
#define PT_DYNAMIC      0x00000002
#define PT_INTERP       0x00000003
#define PT_NOTE         0x00000004
#define PT_SHLIB        0x00000005
#define PT_PHDR         0x00000006
#define PT_TLS          0x00000007
#define PT_LOOS         0x60000000
#define PT_HIOS         0x6FFFFFFF
#define PT_LOPROC       0x70000000
#define PT_HIPROC       0x7FFFFFFF

#define PF_X            0x00000001
#define PF_W            0x00000002
#define PF_R            0x00000004

struct elf32_phdr {
    uint32_t p_type;
    uint32_t p_offset;
    uint32_t p_vaddr;
    uint32_t p_paddr;
    uint32_t p_filesz;
    uint32_t p_memsz;
    uint32_t p_flags;
    uint32_t p_align;
} __attribute__((packed));

struct elf64_phdr {
    uint32_t p_type;
    uint32_t p_flags;
    uint64_t p_offset;
    uint64_t p_vaddr;
    uint64_t p_paddr;
    uint64_t p_filesz;
    uint64_t p_memsz;
    uint64_t p_align;
} __attribute__((packed));

union elf_phdr {
    struct elf32_phdr elf32;
    struct elf64_phdr elf64;
};

#define SHT_NULL            0x00000000
#define SHT_PROGBITS        0x00000001
#define SHT_SYMTAB          0x00000002
#define SHT_STRTAB          0x00000003
#define SHT_RELA            0x00000004
#define SHT_HASH            0x00000005
#define SHT_DYNAMIC         0x00000006
#define SHT_NOTE            0x00000007
#define SHT_NOBITS          0x00000008
#define SHT_REL             0x00000009
#define SHT_SHLIB           0x0000000A
#define SHT_DYNSYM          0x0000000B
#define SHT_INIT_ARRAY      0x0000000E
#define SHT_FINI_ARRAY      0x0000000F
#define SHT_PREINIT_ARRAY   0x00000010
#define SHT_GROUP           0x00000011
#define SHT_SYMTAB_SHNDX    0x00000012
#define SHT_LOOS            0x60000000
#define SHT_HIOS            0x6FFFFFFF
#define SHT_LOPROC          0x70000000
#define SHT_HIPROC          0x7FFFFFFF

#define SHF_WRITE               0x00000001
#define SHF_ALLOC               0x00000002
#define SHF_EXECINSTR           0x00000004
#define SHF_MERGE               0x00000010
#define SHF_STRINGS             0x00000020
#define SHF_INFO_LINK           0x00000040
#define SHF_LINK_ORDER          0x00000080
#define SHF_OS_NONCONFORMING    0x00000100
#define SHF_GROUP               0x00000200
#define SHF_TLS                 0x00000400
#define SHF_MASKOS              0x0FF00000
#define SHF_MASKPROC            0xF0000000

struct elf32_shdr {
    uint32_t sh_name;
    uint32_t sh_type;
    uint32_t sh_flags;
    uint32_t sh_addr;
    uint32_t sh_offset;
    uint32_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint32_t sh_addralign;
    uint32_t sh_entsize;
} __attribute__((packed));

struct elf64_shdr {
    uint32_t sh_name;
    uint32_t sh_type;
    uint64_t sh_flags;
    uint64_t sh_addr;
    uint64_t sh_offset;
    uint64_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint64_t sh_addralign;
    uint64_t sh_entsize;
} __attribute__((packed));

union elf_shdr {
    struct elf32_shdr elf32;
    struct elf64_shdr elf64;
};

struct elf_file {
    union elf_hdr header;
    union elf_phdr current_phdr;
    union elf_shdr current_shdr;
};

// elf32 functions
const struct elf32_phdr *elf32_get_next_phdr(struct elf_file *elf);
const struct elf32_shdr *elf32_get_next_shdr(struct elf_file *elf);

// elf64 functions
#ifdef __ELF64
const struct elf64_phdr *elf32_get_next_phdr(struct elf_file *elf);
const struct elf64_shdr *elf32_get_next_shdr(struct elf_file *elf);

#endif  // __ELF64

// common elf functions
const uint8_t *elf_get_ident(struct elf_file *elf);
const union elf_phdr *elf_get_next_phdr(struct elf_file *elf);
const union elf_shdr *elf_get_next_shdr(struct elf_file *elf);
const union elf_phdr *elf_get_phdr(struct elf_file *elf, uint32_t idx);
const union elf_shdr *elf_get_shdr(struct elf_file *elf, uint32_t idx);
void *elf_read_raw(struct elf_file *elf, void *buf, size_t len);
void *elf_read(struct elf_file *elf, union elf_shdr *s, void *buf, size_t len);
void elf_close(struct elf_file *elf);

#endif // __LIBEHBCFW_ELF_H__
