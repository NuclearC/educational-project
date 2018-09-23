#ifndef REGISTERS_HPP_
#define REGISTERS_HPP_

#include <cstdint>
#include <exception>

namespace core {
namespace backend {
namespace cpu {

size_t ModRM_RegisterIndices_1632[8] = {0, 2, 3, 1, 7, 6, 4, 5};
size_t ModRM_RegisterIndices_8[8] = {
    0, 2, 3, 1, 0, 2, 3, 1,
};

class Registers {
private:
  // 16 general purpose registers
  // order is: A, B, C, D, SI, DI, BP, SP, 8
  // 9, 10, 11, 12, 13, 14, 15
  uint64_t regs1[16];

public:
  Registers();
  ~Registers();

  // write to general register
  void write1(size_t data_size, uint8_t reg, 
    uint64_t value);
  // read from general register
  uint64_t read1(uint8_t reg);
};

}
} // namespace backend
} // namespace core

#endif // REGISTERS_HPP_
