#ifndef DRIVERS_OPENGL_HPP_
#define DRIVERS_OPENGL_HPP_

#include <memory>

#include "backend/graphics/graphics.hpp"
#include <Windows.h>
#include <gl/GL.h>

namespace core {
namespace backend {
namespace graphics {
namespace gl {

// OpenGL Graphics Driver
class OpenGLDriver : public GraphicsDriver {
private:
  HDC dc;
  HGLRC glrc;

public:
  OpenGLDriver(const std::shared_ptr<GraphicsWindow> &window_)
      : GraphicsDriver(window_) {}

  void create() override;

  void begin() override;

  void end() override;
};
} // namespace gl
} // namespace graphics
} // namespace backend
} // namespace core

#endif // DRIVERS_OPENGL_HPP_
