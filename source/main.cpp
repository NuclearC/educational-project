#include "main.hpp"
#include <memory>
#include <mutex>
#include <thread>

#include <Zydis/Zydis.h>
#include <inttypes.h>

#undef min
#undef max

#ifdef WIN32
#pragma comment(linker, "\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

std::mutex mutex;

using core::utils::log;
using core::utils::LogLevel;

int main(int argc, char *argv[]) {

  log("Initializing the Libui", LogLevel::kInfo);

  uiInitOptions ui_init_options = {};

  uiInit(&ui_init_options);

  log("Initializing the emulator", LogLevel::kInfo);
  core::gui::Window wnd{};
  core::Emulator emu{};

  wnd.main_wnd.on_file_open = [&](std::string filename) {
    {
      std::lock_guard guard(mutex);
      log("Loading file " + filename, LogLevel::kInfo);
      emu.load_file(std::filesystem::u8path(filename));
    }

    wnd.debug_wnd.on_debug_resume();
  };

  wnd.debug_wnd.on_debug_resume = [&]() -> bool {
    log("Resuming emulation", LogLevel::kInfo);
    wnd.debug_wnd.set_state(false);
    std::lock_guard guard(mutex);
    emu.state(false);
    return true;
  };

  wnd.debug_wnd.on_debug_pause = [&]() -> bool {
    log("Pausing emulation", LogLevel::kInfo);
    wnd.debug_wnd.set_state(true);
    std::lock_guard guard(mutex);
    emu.state(true);
    return true;
  };

  wnd.debug_wnd.set_state(true);

  log("Current state: paused", LogLevel::kInfo);
  emu.state(true);
  emu.reset();

  std::thread emu_thread = std::thread([&emu]() {
    /*
    try {*/
    emu.initialize();
    while (true) {
      std::lock_guard guard(mutex);
      emu.poll();
    }
    //} catch (std::exception e) {
    //  log("Exception thrown: " + std::string(e.what()), LogLevel::kError);
    //}
  });

  uiMain();
  uiUninit();
  return 0;
}
