#ifndef EMULATOR_HPP_
#define EMULATOR_HPP_

#include "backend/cpu/cpu.hpp"
#include "backend/cpu/disasm.hpp"
#include "backend/cpu/vm.hpp"
#include "backend/memory/memory.hpp"
#include "reader.hpp"

namespace core {
class Emulator {
private:
  core::rom::RomInfo rom_info;
  core::backend::cpu::VirtualMachine vm;
  core::backend::cpu::VirtualCpu cpu;
  core::backend::cpu::DisAsm disasm;
  core::backend::memory::MemoryController mcontrol;

public:
  Emulator();
  ~Emulator();

  void reset();
  void poll();
  void load_file(std::filesystem::path path);
};
} // namespace core

#endif // EMULATOR_HPP_
