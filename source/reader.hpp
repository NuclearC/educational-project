#ifndef READER_HPP_
#define READER_HPP_

#include <filesystem>
#include <fstream>
#include <istream>
#include <string>
#include <vector>

#include "utils/logger.hpp"

#include "elf/elf32.hpp"
#include "elf/elf64.hpp"

namespace core {
namespace rom {

typedef unsigned char byte;

struct RomInfo {
  std::vector<char> data;
  elf64::Elf64_Ehdr header;
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
