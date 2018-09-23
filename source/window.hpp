#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <Windows.h>
#include <cassert>

namespace core {
namespace gui {
class Window {
private:
  HWND wnd;
  WNDCLASSEX wnd_class;

  static LRESULT CALLBACK WndProc( HWND hwnd,  UINT msg,
                                   WPARAM wparam,  LPARAM lparam);

public:
  Window();
  ~Window();

  void poll_events();
};
} // namespace gui
} // namespace core

#endif // WINDOW_HPP_
