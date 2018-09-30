#ifndef ELF32_HPP_
#define ELF32_HPP_

namespace core {
namespace elf32 {

// typedefs
typedef unsigned int Elf32_Addr;
typedef unsigned int Elf32_Off;
typedef unsigned short Elf32_Half;
typedef unsigned int Elf32_Word;
typedef signed int Elf32_Sword;

// structures
struct Elf32_Phdr {
  enum P_TYPE : Elf32_Word {
    PT_NULL = 0,
    PT_LOAD = 1,
    PT_DYNAMIC = 2,
    PT_INTERP = 3,
    PT_NOTE = 4,
    PT_SHLIB = 5,
    PT_PHDR = 6,
    PT_TLS = 7,
    PT_LOOS = 0x60000000,
    PT_HIOS = 0x6fffffff,
    PT_LOPROC = 0x70000000,
    PT_HIPROC = 0x7fffffff,
  } p_type;
  Elf32_Off p_offset;
  Elf32_Addr p_vaddr;
  Elf32_Addr p_paddr;
  Elf32_Word p_filesz;
  Elf32_Word p_memsz;
  enum P_FLAGS : Elf32_Word {
    PF_X = 0x1,
    PF_W = 0x2,
    PF_R = 0x4,
    PF_MASKOS = 0x0ff00000,
    PF_MASKPROC = 0xf0000000,
  } p_flags;
  Elf32_Word p_align;
};

struct alignas(16) Elf32_Ident {
  unsigned char magic[4];
  enum EI_CLASS : unsigned char {
    ELFCLASSNONE,
    ELFCLASS32,
    ELFCLASS64
  } ei_class;

  enum EI_DATA : unsigned char {
    ELFDATANONE,
    ELFDATA2LSB,
    ELFDATA2MSB
  } ei_data;
  unsigned char ei_version;

  enum EI_OSABI : unsigned char {
    ELFOSABI_NONE = 0,
    ELFOSABI_HPUX = 1,
    ELFOSABI_NETBSD = 2,
    ELFOSABI_LINUX = 3,
    ELFOSABI_SOLARIS = 6,
    ELFOSABI_AIX = 7,
    ELFOSABI_IRIX = 8,
    ELFOSABI_FREEBSD = 9,
    ELFOSABI_TRU64 = 10,
    ELFOSABI_MODESTO = 11,
    ELFOSABI_OPENBSD = 12,
    ELFOSABI_OPENVMS = 13,
    ELFOSABI_NSK = 14,
  } ei_osabi;

  unsigned char ei_abiversion;
};

struct Elf32_Ehdr {
  Elf32_Ident e_ident;
  enum E_TYPE : Elf32_Half {
    ET_NONE = 0,
    ET_REL,
    ET_EXEC,
    ET_DYN,
    ET_CORE,
    ET_LOOS = 0xfe00,
    ET_HIOS = 0xfeff,
    ET_LOPROC = 0xff00,
    ET_HIPROC = 0xffff
  } e_type;

  enum E_MACHINE : Elf32_Half {
    EM_NONE = 0,
    EM_M32 = 1,
    EM_SPARC = 2,
    EM_386 = 3,
    EM_68K = 4,
    EM_88K = 5,
    EM_486 = 7,
    EM_MIPS = 8,
    EM_S370 = 9,
    EM_MIPS_RS3_LE = 10,
    EM_PARISC = 15,
    EM_VPP500 = 17,
    EM_SPARC32PLUS = 18,
    EM_960 = 19,
    EM_PPC = 20,
    EM_PPC64 = 21,
    EM_S390 = 22,
    EM_V800 = 36,
    EM_FR20 = 37,
    EM_RH32 = 38,
    EM_RCE = 39,
    EM_ARM = 40,
    EM_ALPHA = 41,
    EM_SH = 42,
    EM_SPARCV9 = 43,
    EM_TRICORE = 44,
    EM_ARC = 45,
    EM_H8_300 = 46,
    EM_H8_300H = 47,
    EM_H8S = 48,
    EM_H8_500 = 49,
    EM_IA_64 = 50,
    EM_MIPS_X = 51,
    EM_COLDFIRE = 52,
    EM_68HC12 = 53,
    EM_MMA = 54,
    EM_PCP = 55,
    EM_NCPU = 56,
    EM_NDR1 = 57,
    EM_STARCORE = 58,
    EM_ME16 = 59,
    EM_ST100 = 60,
    EM_TINYJ = 61,
    EM_X86_64 = 62,
    EM_PDSP = 63,
    EM_PDP10 = 64,
    EM_PDP11 = 65,
    EM_FX66 = 66,
    EM_ST9PLUS = 67,
    EM_ST7 = 68,
    EM_68HC16 = 69,
    EM_68HC11 = 70,
    EM_68HC08 = 71,
    EM_68HC05 = 72,
    EM_SVX = 73,
    EM_ST19 = 74,
    EM_VAX = 75,
    EM_CRIS = 76,
    EM_JAVELIN = 77,
    EM_FIREPATH = 78,
    EM_ZSP = 79,
    EM_MMIX = 80,
    EM_HUANY = 81,
    EM_PRISM = 82,
    EM_AVR = 83,
    EM_FR30 = 84,
    EM_D10V = 85,
    EM_D30V = 86,
    EM_V850 = 87,
    EM_M32R = 88,
    EM_MN10300 = 89,
    EM_MN10200 = 90,
    EM_PJ = 91,
    EM_OPENRISC = 92,
    EM_ARC_A5 = 93,
    EM_XTENSA = 94,
    EM_VIDEOCORE = 95,
    EM_TMM_GPP = 96,
    EM_NS32K = 97,
    EM_TPC = 98,
    EM_SNP1K = 99,
    EM_ST200 = 100,
  } e_machine;

  enum E_VERSION : Elf32_Word { EV_NONE, EV_CURRENT } e_version;

  Elf32_Addr e_entry;
  Elf32_Off e_phoff;
  Elf32_Off e_shoff;
  Elf32_Word e_flags;
  Elf32_Half e_ehsize;
  Elf32_Half e_phentsize;
  Elf32_Half e_phnum;
  Elf32_Half e_shentsize;
  Elf32_Half e_shnum;
  Elf32_Half e_shstrndx;
};

struct Elf32_Shdr {
  Elf32_Word sh_name;
  enum SH_TYPE : Elf32_Word {
    SHT_NULL = 0,
    SHT_PROGBITS = 1,
    SHT_SYMTAB = 2,
    SHT_STRTAB = 3,
    SHT_RELA = 4,
    SHT_HASH = 5,
    SHT_DYNAMIC = 6,
    SHT_NOTE = 7,
    SHT_NOBITS = 8,
    SHT_REL = 9,
    SHT_SHLIB = 10,
    SHT_DYNSYM = 11,
    SHT_INIT_ARRAY = 14,
    SHT_FINI_ARRAY = 15,
    SHT_PREINIT_ARRAY = 16,
    SHT_GROUP = 17,
    SHT_SYMTAB_SHNDX = 18,
    SHT_LOOS = 0x60000000,
    SHT_HIOS = 0x6fffffff,
    SHT_LOPROC = 0x70000000,
    SHT_HIPROC = 0x7fffffff,
    SHT_LOUSER = 0x80000000,
    SHT_HIUSER = 0xffffffff,
  } sh_type;

  enum SH_FALGS : Elf32_Word {
    SHF_WRITE = 0x1,
    SHF_ALLOC = 0x2,
    SHF_EXECINSTR = 0x4,
    SHF_MERGE = 0x10,
    SHF_STRINGS = 0x20,
    SHF_INFO_LINK = 0x40,
    SHF_LINK_ORDER = 0x80,
    SHF_OS_NONCONFORMING = 0x100,
    SHF_GROUP = 0x200,
    SHF_TLS = 0x400,
    SHF_MASKOS = 0x0ff00000,
    SHF_MASKPROC = 0xf0000000,
  } sh_flags;

  Elf32_Addr sh_addr;
  Elf32_Off sh_offset;
  Elf32_Word sh_size;
  Elf32_Word sh_link;
  Elf32_Word sh_info;
  Elf32_Word sh_addralign;
  Elf32_Word sh_entsize;
};

struct Elf32_Sym {
  Elf32_Word st_name;
  Elf32_Addr st_value;
  Elf32_Word st_size;
  unsigned char st_info;
  unsigned char st_other;
  Elf32_Half st_shndx;
};

} // namespace elf32
} // namespace core

#endif // ELF32_HPP_
