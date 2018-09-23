#include "backend/memory/memory.hpp"

namespace core {
namespace backend {
namespace memory {

MemoryController::MemoryController() {
  blocks.emplace_back(MemoryBlock{std::malloc(MB(128)), 0, MB(128)});
}

MemoryController::~MemoryController() {}

void MemoryController::write(uint64_t location, const void *source,
                             size_t size) {
  std::memcpy((void *)((char *)blocks[0].data + location), source, size);
}

void MemoryController::read(uint64_t location, void *dest, size_t size) {
  std::memcpy(dest, (void *)((char *)blocks[0].data + location), size);
}

void *MemoryController::operator[](uint64_t location) {
  return (void *)((char *)blocks[0].data + location);
}

} // namespace memory
} // namespace backend
} // namespace core
