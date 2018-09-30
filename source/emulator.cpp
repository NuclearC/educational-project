#include "emulator.hpp"

namespace core {
Emulator::Emulator()
    : mcontrol{}, cpu(mcontrol), disasm(cpu, vm), rom_info{},
      vm(cpu, mcontrol, this) {}

Emulator::~Emulator() {}

void Emulator::state(bool paused_) { paused = paused_; }

void Emulator::reset() { cpu.reset(); }

void Emulator::poll() {
  if (!paused) {
    disasm.poll();
    cpu.poll();
  }
}

void Emulator::load_file(std::filesystem::path path) {
  rom_info = rom::Reader::read(path);

  if (rom_info.data.empty()) {
    throw std::exception("Invalid ELF64 file");
    return;
  }

  auto &header = rom_info.header;
  auto &program_headers = rom_info.program_header_table;
  auto &section_headers = rom_info.section_header_table;

  for (auto &header : program_headers) {
    switch (header.p_type) {
    case elf64::Elf64_Phdr::PT_LOAD: {
      mcontrol.write(header.p_vaddr, rom_info.data.data() + header.p_offset,
                     header.p_memsz);
    } break;
    }
  }

  cpu.set_inst_pointer(header.e_entry);
}

} // namespace core
