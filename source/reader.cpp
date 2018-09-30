#include "reader.hpp"

namespace core {
namespace rom {
RomInfo Reader::read(std::filesystem::path path) {

  using utils::LogLevel;
  using utils::cout_info;
  using utils::log;

  RomInfo res;

  std::ifstream in(path, std::ios::in | std::ios::binary);

  std::copy(std::istreambuf_iterator<char>(in),
            std::istreambuf_iterator<char>(), std::back_inserter(res.data));

  res.header = {};

  log("Reading ELF64 headers", LogLevel::kInfo);
  const auto elf_pos = 0;
  in.seekg(elf_pos + 0x0);
  in.read((char *)&res.header, sizeof res.header);
  if (strncmp((const char *)res.header.e_ident.magic + 1, "ELF", 3) != 0) {
    log("Invalid ELF64 signature", LogLevel::kError);
    return {};
  }

  if (res.header.e_phoff) {
    log("Reading program header tables", LogLevel::kInfo);
    in.seekg(elf_pos + res.header.e_phoff);
    // read program header table
    for (auto num = 0; num < res.header.e_phnum; num++) {
      elf64::Elf64_Phdr program_header_entry = {};
      in.read((char *)&program_header_entry, res.header.e_phentsize);
      res.program_header_table.push_back(program_header_entry);
    }
  }

  if (res.header.e_shoff) {
    log("Reading section header tables", LogLevel::kInfo);
    in.seekg(elf_pos + res.header.e_shoff);
    // read section header table
    for (auto num = 0; num < res.header.e_shnum; num++) {
      elf64::Elf64_Shdr section_header_entry = {};
      in.read((char *)&section_header_entry, res.header.e_shentsize);
      res.section_header_table.push_back(section_header_entry);
    }
  }

  in.close();

  return res;
}
} // namespace rom
} // namespace core
