#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <cassert>
#include <functional>
#include <string>

#include <ui.h>

namespace core {
namespace gui {

class MainWindow {
private:
  uiWindow *wnd;

public:
  MainWindow();
  ~MainWindow();

  std::function<void(std::string)> on_file_open;
};

class DebugWindow {
private:
  uiWindow *wnd;
  uiButton *pause_button;
  uiButton *resume_button;

public:
  DebugWindow();
  ~DebugWindow();

  void set_state(bool paused);

  std::function<bool()> on_debug_pause;
  std::function<bool()> on_debug_resume;
};

class Window {
public:
  MainWindow main_wnd;
  DebugWindow debug_wnd;

private:

public:
  Window();
  ~Window();
};
} // namespace gui
} // namespace core

#endif // WINDOW_HPP_
