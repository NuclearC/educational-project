#include "backend/cpu/vm.hpp"

namespace core {
namespace backend {
namespace cpu {
bool VirtualMachine::mov_(const ZydisDecodedInstruction &inst) { 
  if (inst.operandCount == 2) {
    auto &move_to = inst.operands[0];
    auto &move_to_type = inst.operands[0].type;
    auto &move_from = inst.operands[1];
    auto &move_from_type = inst.operands[1].type;

    uint64_t value = 0;

    switch (move_from_type) { 
    case ZYDIS_OPERAND_TYPE_IMMEDIATE:
      value = move_from.imm.value.u;
      break;
    }

    switch (move_to_type) { 
    case ZYDIS_OPERAND_TYPE_REGISTER:
      const auto reg = registers::ZydisRegisterIndices[(uint32_t)move_to.reg.value];
      cpu.regs.write_general(reg, value);
      break;
    }

    return true;
  } else // should never fckin happen
    return false;
}

bool VirtualMachine::syscall(const ZydisDecodedInstruction &inst) {
  return true;
}

VirtualMachine::VirtualMachine(VirtualCpu &cpu_,
                               memory::MemoryController &mcontrol_)
    : cpu(cpu_), mcontrol(mcontrol_), inst_mappings{} {
  inst_mappings[ZYDIS_MNEMONIC_MOV] = &VirtualMachine::mov_;
  inst_mappings[ZYDIS_MNEMONIC_SYSCALL] = &VirtualMachine::syscall;
}

VirtualMachine::~VirtualMachine() {}

bool VirtualMachine::execute_instruction(const ZydisDecodedInstruction &inst) {
  auto func = inst_mappings[inst.mnemonic];
  if (func == nullptr)
    return false;
  return (this->*func)(inst);
}
} // namespace cpu
} // namespace backend
} // namespace core

