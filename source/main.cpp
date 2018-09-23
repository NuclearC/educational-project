#include "main.hpp"
#include "backend/memory/memory.hpp"
#include "backend/cpu/cpu.hpp"
#include "backend/cpu/disasm.hpp"
#include "reader.hpp"

#include <Zydis/Zydis.h>
#include <inttypes.h>

#undef min
#undef max

int main(int argc, char *argv[]) {
  core::gui::Window wnd{};
  core::rom::RomInfo info = core::rom::Reader::read(std::filesystem::u8path(
      R"(E:\My Downloads\Sonic Mania v1.03\PS4_Sonic_Mania_CUSA07023_01_Game_Full_psgames.by\CUSA07023\eboot.bin)"));
  core::backend::cpu::VirtualCpu cpu{};
  core::backend::cpu::DisAsm disasm(cpu);
  core::backend::memory::MemoryController mcontrol{};

  const auto pointer = info.data.data() + 0x160 + info.header.e_entry;
  const auto instruction =
      disasm.try_decode((uint64_t)pointer, pointer, info.data.size());

  disasm.print(instruction);

  disasm.try_execute(instruction);

  while (true) {
    wnd.poll_events();
  }

  return 0;
}
