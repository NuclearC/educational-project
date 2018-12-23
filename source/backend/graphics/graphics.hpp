#ifndef GRAPHICS_HPP_
#define GRAPHICS_HPP_

#include <memory>

#include "backend/graphics/gfxwindow.hpp"

namespace core {
namespace backend {
namespace gpu {}
namespace graphics {

// Abstract class for driver
class GraphicsDriver {
protected:
  std::shared_ptr<GraphicsWindow> window;
public:
  GraphicsDriver(const std::shared_ptr<GraphicsWindow> &window_);
  virtual ~GraphicsDriver();

  virtual void create() = 0;

  virtual void begin() = 0;
  virtual void end() = 0;
};
} // namespace graphics
} // namespace backend
} // namespace core

#endif // GRAPHICS_HPP_
