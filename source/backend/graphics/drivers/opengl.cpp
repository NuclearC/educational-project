#include "backend/graphics/drivers/opengl.hpp"

namespace core {
namespace backend {
namespace graphics {
namespace gl {

void OpenGLDriver::create() {
  dc = GetDC((HWND)window->get_window_handle());

  PIXELFORMATDESCRIPTOR pfd = {
      sizeof(PIXELFORMATDESCRIPTOR),
      1,
      PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, // Flags
      PFD_TYPE_RGBA, // The kind of framebuffer. RGBA or palette.
      32,            // Colordepth of the framebuffer.
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      24, // Number of bits for the depthbuffer
      8,  // Number of bits for the stencilbuffer
      0,  // Number of Aux buffers in the framebuffer.
      PFD_MAIN_PLANE,
      0,
      0,
      0,
      0};

  int pixel_format = ChoosePixelFormat(dc, &pfd);
  SetPixelFormat(dc, pixel_format, &pfd);

  glrc = wglCreateContext(dc);
  wglMakeCurrent(dc, glrc);

  window->set_title("OpenGL Graphics Output");
}

void OpenGLDriver::begin() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0, 0, 0, 1);
}

void OpenGLDriver::end() { SwapBuffers(dc); }

} // namespace gl
} // namespace graphics
} // namespace backend
} // namespace core
