#ifndef READER_HPP_
#define READER_HPP_

#include <filesystem>
#include <fstream>
#include <istream>
#include <string>
#include <vector>

namespace core {
namespace rom {

typedef unsigned char byte;

namespace elf64 {
typedef unsigned long long Elf64_Addr;
typedef unsigned long long Elf64_Off;
typedef unsigned short Elf64_Half;
typedef unsigned int Elf64_Word;
typedef signed int Elf64_Sword;
typedef unsigned long long Elf64_Xword;
typedef signed long long Elf64_Sxword;

struct Elf64_IdentS {
  uint8_t signature[4];
  enum EI_CLASS : uint8_t { ELFCLASSNONE, ELFCLASS32, ELFCLASS64 } ei_class;
  enum EI_DATA : uint8_t { ELFDATANONE, ELFDATA2LSB, ELFDATA2MSB } ei_data;
  enum EI_VERSION : uint8_t { EV_CURRENT = (uint8_t)1 } ei_version;
  enum EI_OSABI : uint8_t {
    ELFOSABI_NONE,
    ELFOSABI_HPUX,
    ELFOSABI_NETBSD,
    ELFOSABI_GNU,
    ELFOSABI_SOLARIS = (uint8_t)6,
    ELFOSABI_AIX,
    ELFOSABI_IRIX,
    ELFOSABI_FREEBSD,
    ELFOSABI_TRU64,
    ELFOSABI_MODESTO,
    ELFOSABI_OPENBSD,
    ELFOSABI_OPENVMS,
    ELFOSABI_NSK,
    ELFOSABI_AROS,
    ELFOSABI_FENIXOS,
    ELFOSABI_CLOUDABI,
    ELFOSABI_OPENVOS
  } ei_osabi;
  uint8_t ei_abiversion;
  char padding[7];
}; // namespace elf64

struct Elf64_HeaderS {
  Elf64_IdentS e_ident; /* ELF identification */
  enum E_TYPE : Elf64_Half {
    ET_NONE,
    ET_REL,
    ET_EXEC,
    ET_DYN,
    ET_CORE,
    ET_LOOS = 65024,
    ET_HIOS = 65279,
    ET_LOPROC = 65280,
    ET_HIPROC = 65535
  } e_type; /* Object file type */
  enum E_MACHINE : Elf64_Half {
    EM_NONE = (Elf64_Half)0,
    EM_386 = (Elf64_Half)3,
    EM_PPC = (Elf64_Half)20,
    EM_PPC64 = (Elf64_Half)21,
    EM_X86_64 = (Elf64_Half)62,
  } e_machine; /* Machine type */
  enum E_VERSION : Elf64_Word {
    EV_NONE,
    EV_CURRENT
  } e_version;            /* Object file version */
  Elf64_Addr e_entry;     /* Entry point address */
  Elf64_Off e_phoff;      /* Program header offset */
  Elf64_Off e_shoff;      /* Section header offset */
  Elf64_Word e_flags;     /* Processor-specific flags */
  Elf64_Half e_ehsize;    /* ELF header size */
  Elf64_Half e_phentsize; /* Size of program header entry */
  Elf64_Half e_phnum;     /* Number of program header entries */
  Elf64_Half e_shentsize; /* Size of section header entry */
  Elf64_Half e_shnum;     /* Number of section header entries */
  Elf64_Half e_shstrndx;  /* Section name string table index */
};

struct Elf64_Phdr {
  enum P_TYPE : Elf64_Word {
    PT_NULL,
    PT_LOAD,
    PT_DYNAMIC,
    PT_INTERP,
    PT_NOTE,
    PT_SHLIB,
    PT_PHDR,
    PT_LOOS = 0x60000000,
    PT_HIOS = 0x6fffffff,
    PT_LOPROC = 0x70000000,
    PT_HIPROC = 0x7fffffff
  } p_type;
  uint32_t p_flags;
  Elf64_Off p_offset;
  Elf64_Addr p_vaddr;
  Elf64_Addr p_paddr;
  uint64_t p_filesz;
  uint64_t p_memsz;
  uint64_t p_align;
};

struct Elf64_Shdr {
  uint32_t sh_name;
  uint32_t sh_type;
  uint64_t sh_flags;
  Elf64_Addr sh_addr;
  Elf64_Off sh_offset;
  uint64_t sh_size;
  uint32_t sh_link;
  uint32_t sh_info;
  uint64_t sh_addralign;
  uint64_t sh_entsize;
};

struct Elf64_Sym {
  uint32_t st_name;
  unsigned char st_info;
  unsigned char st_other;
  uint16_t st_shndx;
  Elf64_Addr st_value;
  uint64_t st_size;
};

} // namespace elf64

struct RomInfo {
  std::vector<char> data;
  elf64::Elf64_HeaderS header;
  std::vector<elf64::Elf64_Phdr> program_header_table;
  std::vector<elf64::Elf64_Shdr> section_header_table;
};

class Reader {
public:
  static RomInfo read(std::filesystem::path path);
};
} // namespace rom
} // namespace core

#endif // READER_HPP_
