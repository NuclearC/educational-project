#include "backend/cpu/registers.hpp"


namespace core {
namespace backend {
namespace cpu {

Registers::Registers() : regs1{} {}
Registers::~Registers() {}

void Registers::write1(size_t data_size, uint8_t reg, uint64_t value) {
  if (reg > 8)
    throw std::exception("bad register");

  if (data_size == 32 || data_size == 16 || data_size == 64)
    regs1[ModRM_RegisterIndices_1632[reg]] = value & (data_size - 1);
  else if (data_size == 8)
    regs1[ModRM_RegisterIndices_8[reg]] = value & 0xff;
  else
    throw std::exception("bad data size");
}

uint64_t Registers::read1(uint8_t reg) { return uint64_t(); }

} // namespace cpu
} // namespace backend
} // namespace core

