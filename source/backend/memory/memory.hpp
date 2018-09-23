#ifndef MEMORY_HPP_
#define MEMORY_HPP_

#include <algorithm>
#include <vector>

namespace core {
namespace backend {
namespace memory {

#define KB(x) x * 1024
#define MB(x) KB(KB(x))


struct MemoryBlock {
#ifdef _DEBUG
  // TODO: some debug flags here
#endif // _DEBUG
  void *data;
  uint64_t begin, end;
};

class MemoryController {
private:
  std::vector<MemoryBlock> blocks;

public:
  MemoryController();
  ~MemoryController();

  void write(uint64_t location, const void *data, size_t size);
  void read(uint64_t location, void *dest, size_t size);
  void *operator[](uint64_t location);
};
} // namespace memory
} // namespace backend
} // namespace core

#endif // MEMORY_HPP_
