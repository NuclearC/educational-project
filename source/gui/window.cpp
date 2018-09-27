#include "window.hpp"

namespace core {
namespace gui {

Window::Window() : main_wnd(), debug_wnd() {}

Window::~Window() {}

MainWindow::MainWindow() {
  auto menu = uiNewMenu("&File");
  auto item = uiMenuAppendItem(menu, "&Open ELF\tCtrl+O");
  uiMenuItemOnClicked(item,
                      [](uiMenuItem *, uiWindow *caller_wnd, void *data) {
                        MainWindow *wnd = (MainWindow *)data;
                        auto file = uiOpenFile(caller_wnd);
                        if (wnd->on_file_open != nullptr) {
                          wnd->on_file_open(std::string(file));
                        }
                      },
                      this);

  menu = uiNewMenu("&Debug");
  wnd = uiNewWindow("Emulator", 640, 480, true);
  uiControlShow(uiControl(wnd));
}
MainWindow::~MainWindow() {}
DebugWindow::DebugWindow() {
  wnd = uiNewWindow("Debugger", 640, 320, false);

  auto box = uiNewVerticalBox();
  auto upper_box = uiNewVerticalBox();
  auto lower_box = uiNewVerticalBox();

  { // the controls
    auto group = uiNewGroup("Debugging");
    auto inner_box = uiNewHorizontalBox();
    uiBoxSetPadded(inner_box, 1);
    uiGroupSetMargined(group, 1);
    pause_button = uiNewButton("Pause");
    resume_button = uiNewButton("Resume");
    uiButtonOnClicked(pause_button,
                      [](uiButton *, void *data) {
                        DebugWindow *wnd = (DebugWindow *)data;
                        wnd->on_debug_pause();
                      },
                      this);
    uiButtonOnClicked(resume_button,
                      [](uiButton *, void *data) {
                        DebugWindow *wnd = (DebugWindow *)data;
                        wnd->on_debug_resume();
                      },
                      this);

    uiBoxAppend(inner_box, uiControl(pause_button), 0);
    uiBoxAppend(inner_box, uiControl(resume_button), 0);

    uiGroupSetChild(group, uiControl(inner_box));
    uiBoxAppend(upper_box, uiControl(group), 1);
  }

  { // tabs
    auto tab = uiNewTab();
    { // cpu tab
      auto inner = uiNewHorizontalBox();
      uiTabAppend(tab, "CPU", uiControl(inner));
    }

    { // memory tab
      auto inner = uiNewHorizontalBox();
      uiTabAppend(tab, "Memory", uiControl(inner));
    }

    uiBoxAppend(lower_box, uiControl(tab), 1);
  }

  uiBoxAppend(box, uiControl(upper_box), 0);
  uiBoxAppend(box, uiControl(lower_box), 1);
  uiWindowSetChild(wnd, uiControl(box));

  uiControlShow(uiControl(wnd));
}

DebugWindow::~DebugWindow() {}

void DebugWindow::set_state(bool paused) {
  if (paused) {
    uiControlEnable(uiControl(resume_button));
    uiControlDisable(uiControl(pause_button));
  } else {
    uiControlDisable(uiControl(resume_button));
    uiControlEnable(uiControl(pause_button));
  }
}
} // namespace gui
} // namespace core
