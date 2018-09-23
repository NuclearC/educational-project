#ifndef MEMORY_HPP_
#define MEMORY_HPP_

#include <vector>
#include <algorithm>

namespace core {
namespace backend {
namespace memory {

struct MemoryBlock {
#ifdef _DEBUG
  // TODO: some debug flags here
#endif // _DEBUG
  void *data;
  size_t size;
  uint64_t virtual_offset;
};

class MemoryController {
private:
  std::vector<MemoryBlock> allocated_blocks;

  size_t total_used;

public:
  MemoryController();
  ~MemoryController();

  const MemoryBlock &allocate_block(size_t size, uint64_t virtual_offset);
  void free_block(const MemoryBlock &block);

  void write_virtual_memory(const void *source, size_t source_size,
                            uint64_t destination_offset);
  void write_virtual_memory(const void *source, size_t source_size,
                            MemoryBlock &block, uint64_t block_offset);
  void read_virtual_memory(void *destination, size_t size, uint64_t source);
  void read_virtual_memory(void *destination, size_t size,
                            MemoryBlock &block, uint64_t block_offset);
};
} // namespace memory
} // namespace backend
} // namespace core

#endif // MEMORY_HPP_
