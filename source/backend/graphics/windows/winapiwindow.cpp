#include "backend/graphics/windows/winapiwindow.hpp"

namespace core {
namespace backend {
namespace graphics {
namespace win {
LRESULT CALLBACK WinApiGraphicsWindow::WndProc(HWND hWnd, UINT message,
                                               WPARAM wParam, LPARAM lParam) {
  switch (message) {
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
    break;
  }

  return 0;
}

void WinApiGraphicsWindow::create() {
  // pro way for getting HINSTANCE
  const auto hinst = GetModuleHandle(0);

  // init to 0
  wndclass = {};
  wndclass.cbSize = sizeof(WNDCLASSEX);
  wndclass.style = CS_HREDRAW | CS_VREDRAW;
  wndclass.lpfnWndProc = WndProc;
  wndclass.cbClsExtra = 0;
  wndclass.cbWndExtra = 0;
  wndclass.hInstance = hinst;
  wndclass.hIcon = LoadIcon(hinst, IDI_APPLICATION);
  wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
  wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW);
  wndclass.lpszMenuName = NULL;
  wndclass.lpszClassName = "MlgWindowClass";
  wndclass.hIconSm = LoadIcon(hinst, IDI_APPLICATION);

  // Register the window class
  if (!RegisterClassEx(&wndclass)) 
    // pro error handler
    throw std::exception("failed to register the window class");
  
  // have to calculate xy at the center, and client size
  int window_x, window_y;
  RECT window_rect{0,0,640, 480};
  AdjustWindowRectEx(&window_rect, WS_OVERLAPPEDWINDOW, false,
                     WS_EX_OVERLAPPEDWINDOW);


  RECT rect{};
  GetClientRect(GetDesktopWindow(), &rect);

  window_x = rect.right / 2 - (window_rect.right - window_rect.left) / 2;
  window_y = rect.bottom / 2 - (window_rect.bottom - window_rect.top) / 2;

  // create the window
  hwnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, "MlgWindowClass", "GFXWindow",
                        WS_OVERLAPPEDWINDOW, window_x, window_y,
                        window_rect.right - window_rect.left,
                        window_rect.bottom - window_rect.top, 0, 0,
                        hinst, 0);
  if (!hwnd) // mlg error handling
    throw std::exception("failed to create the window");

  // show and update the window
  ShowWindow(hwnd, SW_SHOW);
  UpdateWindow(hwnd);
}

void WinApiGraphicsWindow::run() {
  window_thread = std::thread([this] {
    while (true) {
      poll();
    }
  });
}

void WinApiGraphicsWindow::release() {}

void WinApiGraphicsWindow::poll() {
  MSG msg;
  if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

void WinApiGraphicsWindow::set_title(std::string_view str) {
  SetWindowText(hwnd, str.data());
}

void *WinApiGraphicsWindow::get_window_handle() { return (void*)hwnd; }

} // namespace win
} // namespace graphics
} // namespace backend
} // namespace core
