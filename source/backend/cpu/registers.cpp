#include "backend/cpu/registers.hpp"


namespace core {
namespace backend {
namespace cpu {
namespace registers {
void Registers::write_general(uint8_t reg, uint64_t value) { 
  if (reg & 16) {
    regs1[reg & ~16] = value & 0xffffffff; 
  } else if (reg & 32) {
    regs1[reg & ~32] = value & 0xffff; 
  } else if (reg & 64) {
    regs1[reg & ~64] = value & 0xff; 
  } else if (reg & 128) {
    regs1[reg & ~128] = value & 0x00ff; 
  } else {
    regs1[reg] = value; 
  }
}

uint64_t Registers::read_general(uint8_t reg) {
  if (reg & 16) {
    return regs1[reg & ~16] & 0xffffffff;
  } else if (reg & 32) {
    return regs1[reg & ~32] & 0xffff;
  } else if (reg & 64) {
    return regs1[reg & ~64] & 0xff;
  } else if (reg & 128) {
    return regs1[reg & ~128] & 0x00ff;
  } else {
    return regs1[reg];
  }
}

Registers::Registers() : regs1{}, regs2{}, rip{}, rflags{} {}
Registers::~Registers() {}
} // namespace registers

} // namespace cpu
} // namespace backend
} // namespace core

