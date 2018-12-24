#include "emulator.hpp"

namespace core {

  
void Emulator::create_graphics_window() {
  using namespace core::utils;
  log("creating graphics system", kInfo);

  // create graphics window first
  graphics_window =
      std::make_shared<core::backend::graphics::win::WinApiGraphicsWindow>();

  graphics_window->create();

  graphics_window->run();
}

void Emulator::create_graphics_driver() {
  graphics_driver = std::make_unique<core::backend::graphics::gl::OpenGLDriver>(
      graphics_window);
  graphics_driver->create();
}

Emulator::Emulator()
    : mcontrol{}, cpu(mcontrol), disasm(cpu, vm), rom_info{},
      vm(cpu, mcontrol, this), gpu{} {
  create_graphics_window();
}

Emulator::~Emulator() {}

void Emulator::state(bool paused_) { paused = paused_; }

void Emulator::initialize() {
  create_graphics_driver();

  cpu.reset();
  gpu.reset();
}

void Emulator::reset() {
  gpu.reset();
  cpu.reset();
}

void Emulator::poll() {
  if (!paused) {
    disasm.poll();
    cpu.poll();

    graphics_driver->begin();

    graphics_driver->end();
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

  for (auto &program_header : program_headers) {
    switch (program_header.p_type) {
    case elf64::Elf64_Phdr::PT_LOAD: {
      mcontrol.write(program_header.p_vaddr,
                     rom_info.data.data() + program_header.p_offset,
                     program_header.p_memsz);
    } break;
    }
  }

  cpu.set_inst_pointer(header.e_entry);
  cpu.set_base_pointer(0x00);
  cpu.set_stack_pointer(MB(64));
}

} // namespace core
