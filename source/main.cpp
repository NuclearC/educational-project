#include "main.hpp"
#include <memory>

#include <Zydis/Zydis.h>
#include <inttypes.h>

#undef min
#undef max

int main(int argc, char *argv[]) {
  core::gui::Window wnd{};
  core::Emulator emu{};

  emu.reset();
  emu.load_file(std::filesystem::u8path("E:\\Server-Out\\test.elf"));
  
  while (true) {
    wnd.poll_events();
    emu.poll();
  }

  return 0;
}
