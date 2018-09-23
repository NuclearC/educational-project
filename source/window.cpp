#include "window.hpp"

namespace core {
namespace gui {
LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

  switch (msg) {
  default:
    return DefWindowProc(hwnd, msg, wparam, lparam);
  }
}

Window::Window() {
  const auto inst = GetModuleHandle(0);
  wnd_class = {};
  wnd_class.cbSize = sizeof(WNDCLASSEX);
  wnd_class.style = CS_HREDRAW | CS_VREDRAW;
  wnd_class.lpfnWndProc = Window::WndProc;
  wnd_class.cbClsExtra = 0;
  wnd_class.cbWndExtra = 0;
  wnd_class.hInstance = inst;
  wnd_class.hIcon = LoadIcon(inst, MAKEINTRESOURCE(IDI_APPLICATION));
  wnd_class.hCursor = LoadCursor(NULL, IDC_ARROW);
  wnd_class.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wnd_class.lpszMenuName = NULL;
  wnd_class.lpszClassName = "WindowClass";
  wnd_class.hIconSm =
      LoadIcon(wnd_class.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));  

  assert(RegisterClassEx(&wnd_class));

  RECT rect = {0, 0, 640, 480};
  AdjustWindowRect(&rect, WS_OVERLAPPED, true);

  wnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, "WindowClass", "Emulator", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left,
                       rect.bottom - rect.top, 0, 0, inst, 0);
  auto err = GetLastError();
  ShowWindow(wnd, SW_SHOW);
  UpdateWindow(wnd);
}

Window::~Window() {}
void Window::poll_events() {
  MSG msg;
  if (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}
} // namespace gui
} // namespace core

