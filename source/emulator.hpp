#ifndef EMULATOR_HPP_
#define EMULATOR_HPP_

#include "backend/cpu/cpu.hpp"
#include "backend/cpu/disasm.hpp"
#include "backend/cpu/vm.hpp"
#include "backend/graphics/drivers/opengl.hpp"
#include "backend/graphics/gpu.hpp"
#include "backend/graphics/graphics.hpp"
#include "backend/graphics/gfxwindow.hpp"
#include "backend/graphics/windows/winapiwindow.hpp"
#include "backend/kernel/kernel.hpp"

#include "backend/memory/memory.hpp"
#include "reader.hpp"

namespace core {
class Emulator {
private:
  core::rom::RomInfo rom_info;
  core::backend::gpu::VirtualGpu gpu;
  core::backend::cpu::VirtualMachine vm;
  core::backend::cpu::VirtualCpu cpu;
  core::backend::cpu::DisAsm disasm;
  core::backend::memory::MemoryController mcontrol;
  core::kernel::Kernel krnl;

  std::shared_ptr<core::backend::graphics::GraphicsWindow> graphics_window;
  std::unique_ptr<core::backend::graphics::GraphicsDriver> graphics_driver;

  bool paused;

  void create_graphics_window();
  void create_graphics_driver();

public:
  Emulator();
  ~Emulator();

  void state(bool paused_);
  void initialize();
  void reset();
  void poll();
  void load_file(std::filesystem::path path);
};
} // namespace core

#endif // EMULATOR_HPP_
