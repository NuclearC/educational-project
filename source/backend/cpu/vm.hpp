#ifndef VM_HPP_
#define VM_HPP_

#include <variant>

#include "backend/cpu/cpu.hpp"
#include "backend/memory/memory.hpp"

#include <Zydis/Zydis.h>

namespace core {
namespace backend {
namespace cpu {
class VirtualMachine {
private:
  VirtualCpu &cpu;
  memory::MemoryController &mcontrol;

  bool mov_(const ZydisDecodedInstruction &inst);
  bool syscall(const ZydisDecodedInstruction &inst);

  typedef bool (VirtualMachine::*InstructionMapping)(
      const ZydisDecodedInstruction&);
  InstructionMapping inst_mappings[1582];

public:
  VirtualMachine(VirtualCpu &cpu_, memory::MemoryController &mcontrol_);
  ~VirtualMachine();

  bool execute_instruction(const ZydisDecodedInstruction &inst);
};
} // namespace cpu
} // namespace backend
} // namespace core

#endif // VM_HPP_
