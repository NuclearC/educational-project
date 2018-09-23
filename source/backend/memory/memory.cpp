#include "backend/memory/memory.hpp"

namespace core {
namespace backend {
namespace memory {
MemoryController::MemoryController() {

}

MemoryController::~MemoryController() {
  for (auto &&block : allocated_blocks) {
    delete[] block.data;
  }
  allocated_blocks.clear();
}

const MemoryBlock &MemoryController::allocate_block(size_t size,
                                                    uint64_t virtual_offset) {
  total_used += size;
  return allocated_blocks.emplace_back(
      MemoryBlock{std::malloc(size), size, virtual_offset});
}

void MemoryController::free_block(const MemoryBlock &block) {
  auto it = std::find_if(allocated_blocks.begin(), allocated_blocks.end(),
                         [&block](const MemoryBlock &_block) {
                           return _block.data == block.data;
                         });
  if (it != allocated_blocks.end()) {
    total_used -= block.size;
    delete[] block.data;
    allocated_blocks.erase(it);
  } else {
    // wtf should never happen
    throw std::exception("freeing unallocated/unknown memory block");
  }
}

void MemoryController::write_virtual_memory(const void *source,
                                            size_t source_size,
                                            uint64_t destination_offset) {
  // try to find the block which has the bounds
  // within the given addresses
  bool found = false;
  for (auto &&block : allocated_blocks) {
    if (destination_offset >= block.virtual_offset &&
        destination_offset - block.virtual_offset < source_size) {
      found = true;
      write_virtual_memory(source, source_size, block,
                           destination_offset - block.virtual_offset);
    }
  }

  if (!found) {
    // should never happen
    throw std::out_of_range("writing to invalid location");
  }
}

void MemoryController::write_virtual_memory(const void *source,
                                            size_t source_size,
                                            MemoryBlock &block,
                                            uint64_t block_offset) {
  // try to write to existing block
  // check the bounds
  if (block_offset + source_size < block.size) {
    std::memcpy((void *)((char *)block.data + block_offset), source,
                source_size);
  } else {
    // should never happen too
    throw std::out_of_range("writing to invalid location");
  }
}

void MemoryController::read_virtual_memory(void *destination, size_t size,
                                           uint64_t source) {
  // try to find the block which has the bounds
  // within the given addresses
  bool found = false;
  for (auto &&block : allocated_blocks) {
    if (source >= block.virtual_offset &&
        source - block.virtual_offset < size) {
      found = true;
      read_virtual_memory(destination, size, block, source - block.virtual_offset);
    }
  }

  if (!found) {
    // should never happen
    throw std::out_of_range("reading from invalid location");
  }
}

void MemoryController::read_virtual_memory(void *destination, size_t size,
                                           MemoryBlock &block,
                                           uint64_t block_offset) {
  // try to read from existing block
  // check the bounds
  if (block_offset + size < block.size) {
    std::memcpy(destination, (const void*)((const char*)block.data + block_offset),
                size);
  } else {
    // should never happen too
    throw std::out_of_range("reading from invalid location");
  }
}

} // namespace memory
} // namespace backend
} // namespace core
