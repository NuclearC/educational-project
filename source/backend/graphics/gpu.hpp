#ifndef GPU_HPP_
#define GPU_HPP_

namespace core {
namespace backend {
namespace gpu {
class VirtualGpu {
public:
  VirtualGpu();
  ~VirtualGpu();

  void reset();

};
} // namespace gpu
} // namespace backend
} // namespace core

#endif // GPU_HPP_
