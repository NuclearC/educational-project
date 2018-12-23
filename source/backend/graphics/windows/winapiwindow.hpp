#include "backend/graphics/gfxwindow.hpp"

#include <exception>
#include <Windows.h>

namespace core {
namespace backend {
namespace graphics {
namespace win {

class WinApiGraphicsWindow : public GraphicsWindow {
private:
  static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                           LPARAM lParam);

  HWND hwnd;
  WNDCLASSEX wndclass;

public:
  void create() override;
  void run() override;
  void release() override;
  void poll() override;

  void set_title(std::string_view str) override;

  void* get_window_handle() override;
};
} // namespace win
} // namespace graphics
} // namespace backend
} // namespace core
