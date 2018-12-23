#ifndef GFX_WINDOW_HPP_
#define GFX_WINDOW_HPP_

#include <string>
#include <thread>

namespace core {
namespace backend {
namespace graphics {

// Abstract class for window
class GraphicsWindow {
protected:
  std::thread window_thread;

public:
  GraphicsWindow() {}
  virtual ~GraphicsWindow() {}

  virtual void create() = 0;
  virtual void run() = 0;
  virtual void release() = 0;
  virtual void poll() = 0;
  virtual void set_title(std::string_view str) = 0;

  virtual void *get_window_handle() = 0;
};

} // namespace graphics
} // namespace backend
} // namespace core

#endif // GFX_WINDOW_HPP_
