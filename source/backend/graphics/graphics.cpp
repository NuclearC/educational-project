
#include "backend/graphics/graphics.hpp"

namespace core {
namespace backend {
namespace graphics {

GraphicsDriver::GraphicsDriver(const std::shared_ptr<GraphicsWindow> &window_)
    : window(window_) {}

GraphicsDriver::~GraphicsDriver() {}

} // namespace graphics
} // namespace backend
} // namespace core

