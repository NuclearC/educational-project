#include "reader.hpp"

namespace core {
namespace rom {
RomInfo Reader::read(std::filesystem::path path) { 
  RomInfo res;

  std::ifstream in(path, std::ios::in | std::ios::binary);

  std::copy(std::istreambuf_iterator<char>(in),
            std::istreambuf_iterator<char>(), std::back_inserter(res.data));

  res.header = {};
  
  const auto elf_pos = 0x160;
  in.seekg(elf_pos + 0x0);
  in.read((char *)&res.header, sizeof res.header);

  if (res.header.e_phoff) {
    in.seekg(elf_pos + res.header.e_phoff);
    // read program header table
    for (auto num = 0; num < res.header.e_phnum; num++) {
      elf64::Elf64_Phdr program_header_entry = {};
      in.read((char *)&program_header_entry, res.header.e_phentsize);
      res.program_header_table.push_back(program_header_entry);
    }
  }

  if (res.header.e_shoff) {
    in.seekg(elf_pos + res.header.e_shoff);
    // read program section table
    for (auto num = 0; num < res.header.e_shnum; num++) {
      elf64::Elf64_Shdr section_header_entry = {};
      in.read((char *)&section_header_entry, sizeof section_header_entry);
      res.section_header_table.push_back(section_header_entry);
    }
  }

  in.close();

  return res;
}
} // namespace rom
} // namespace core
