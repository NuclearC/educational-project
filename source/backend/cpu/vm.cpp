#include "backend/cpu/vm.hpp"
#include "emulator.hpp"

namespace core {
namespace backend {
namespace cpu {

void VirtualMachine::handle_syscall(int op) {
  switch (op) {
  case 0:
    return krnl.impl_sys_restart_syscall_();
  case 1:
    return krnl.impl_sys_exit_(cpu.reg().read(ZYDIS_REGISTER_EBX));
  case 2:
    return krnl.impl_sys_fork_((void *)cpu.reg().read(ZYDIS_REGISTER_RBX));
  case 3:
    return krnl.impl_sys_read_(cpu.reg().read(ZYDIS_REGISTER_EBX),
                               (char *)cpu.reg().read(ZYDIS_REGISTER_RCX),
                               cpu.reg().read(ZYDIS_REGISTER_EDX));
  case 4:
    return krnl.impl_sys_write_(
        cpu.reg().read(ZYDIS_REGISTER_EBX),
        (const char *)cpu.reg().read(ZYDIS_REGISTER_RCX),
        cpu.reg().read(ZYDIS_REGISTER_RDX));
  case 5:
    return krnl.impl_sys_open_((const char *)cpu.reg().read(ZYDIS_REGISTER_RBX),
                               cpu.reg().read(ZYDIS_REGISTER_ECX),
                               cpu.reg().read(ZYDIS_REGISTER_EDX));
  case 6:
    return krnl.impl_sys_close_(cpu.reg().read(ZYDIS_REGISTER_EBX));
  case 7:
    return krnl.impl_sys_waitpid_(cpu.reg().read(ZYDIS_REGISTER_EBX),
                                  (int *)cpu.reg().read(ZYDIS_REGISTER_RCX),
                                  cpu.reg().read(ZYDIS_REGISTER_EDX));
  case 8:
    return krnl.impl_sys_creat_(
        (const char *)cpu.reg().read(ZYDIS_REGISTER_RBX),
        cpu.reg().read(ZYDIS_REGISTER_ECX));
  case 9:
    return krnl.impl_sys_link_(
        (const char *)cpu.reg().read(ZYDIS_REGISTER_RBX),
        (const char *)cpu.reg().read(ZYDIS_REGISTER_RCX));
  default:
    throw std::exception("unrealized or unknown syscall");
  }
}

uint64_t VirtualMachine::read_operand(const ZydisDecodedOperand &operand) {

  switch (operand.type) {
  case ZydisOperandType::ZYDIS_OPERAND_TYPE_IMMEDIATE: {
    return operand.imm.value.u;
  } break;
  case ZydisOperandType::ZYDIS_OPERAND_TYPE_MEMORY: {
    const auto &mem = operand.mem;
    uint64_t address = 0;

    if (mem.base) {
      address = cpu.regs.read(mem.base);
    }
    if (mem.index) {
      if (mem.scale) {
        address += cpu.regs.read(mem.index) * mem.scale;
      } else {
        address += cpu.regs.read(mem.index);
      }
    }

    if (mem.disp.has_displacement) {
      address += mem.disp.value;
    }

    switch (mem.type) {
    case ZydisMemoryOperandType::ZYDIS_MEMOP_TYPE_AGEN: {
      // generate address
      return address;
    } break;
    case ZydisMemoryOperandType::ZYDIS_MEMOP_TYPE_MEM: {
      // normal memory operand
      uint64_t res;
      mcontrol.read(address, (void *)&res, operand.size / 8);
      return res;
    } break;
    default:
      throw std::exception("invalid memory operand type");
    }
  } break;
  case ZydisOperandType::ZYDIS_OPERAND_TYPE_POINTER: {
    abort();
  } break;
  case ZydisOperandType::ZYDIS_OPERAND_TYPE_REGISTER: {
    return (uint64_t)cpu.regs.read(operand.reg.value);
  } break;
  default:
    throw std::exception("invalid operand type");
  }

  return 0;
}

void VirtualMachine::write_operand(const ZydisDecodedOperand &operand,
                                   uint64_t value) {

  switch (operand.type) {
  case ZydisOperandType::ZYDIS_OPERAND_TYPE_IMMEDIATE: {
    abort(); // not possibru
  } break;
  case ZydisOperandType::ZYDIS_OPERAND_TYPE_MEMORY: {
    const auto &mem = operand.mem;
    uint64_t address = 0;

    if (mem.base) {
      address = cpu.regs.read(mem.base);
    }
    if (mem.index) {
      if (mem.scale) {
        address += cpu.regs.read(mem.index) * mem.scale;
      } else {
        address += cpu.regs.read(mem.index);
      }
    }

    if (mem.disp.has_displacement) {
      address += mem.disp.value;
    }

    switch (mem.type) {
    case ZydisMemoryOperandType::ZYDIS_MEMOP_TYPE_AGEN: {
      // generate address (again not possible)
      abort();
    } break;
    case ZydisMemoryOperandType::ZYDIS_MEMOP_TYPE_MEM: {
      // normal memory operand
      mcontrol.write(address, (void *)&value, operand.size / 8);
    } break;
    default:
      throw std::exception("invalid memory operand type");
    }
  } break;
  case ZydisOperandType::ZYDIS_OPERAND_TYPE_POINTER: {
    abort();
  } break;
  case ZydisOperandType::ZYDIS_OPERAND_TYPE_REGISTER: {
    cpu.regs.write(operand.reg.value, (uint64_t)value);
  } break;
  default:
    throw std::exception("invalid operand type");
  }
}

void VirtualMachine::push(uint64_t value) {
  // get the next stack pointer
  auto sp = cpu.regs.read(ZYDIS_REGISTER_RSP);
  // decrement so we get the new address
  sp -= sizeof value;

  cpu.regs.write(ZYDIS_REGISTER_RSP, sp);
  mcontrol.write(sp, (const void *)&value, sizeof value);
}

uint64_t VirtualMachine::pop() {
  // get the stack pointer
  auto sp = cpu.regs.read(ZYDIS_REGISTER_RSP);

  uint64_t result = 0;
  mcontrol.read(sp, (void *)&result, sizeof result);
  // get the next address by incrementing
  sp += sizeof result;

  cpu.regs.write(ZYDIS_REGISTER_RSP, sp);

  return result;
}

bool VirtualMachine::int_(const ZydisDecodedInstruction &inst) {
  const auto interrupt = read_operand(inst.operands[0]);

  switch (interrupt) {
  case 128: // kernel call
    handle_syscall(cpu.reg().read(ZYDIS_REGISTER_EAX));
    break;
  }

  return true;
}

bool VirtualMachine::mov_(const ZydisDecodedInstruction &inst) {
  write_operand(inst.operands[0], read_operand(inst.operands[1]));
  return true;
}

bool VirtualMachine::lea_(const ZydisDecodedInstruction &inst) {
  write_operand(inst.operands[0], read_operand(inst.operands[1]));
  return true;
}

bool VirtualMachine::call_(const ZydisDecodedInstruction &inst) {
  push(cpu.get_inst_pointer());
  if (inst.operands[0].type == ZYDIS_OPERAND_TYPE_IMMEDIATE) {
    if (inst.operands[0].imm.is_relative)
      cpu.set_inst_pointer(cpu.get_inst_pointer() +
                           inst.operands[0].imm.value.s);
    else
      cpu.set_inst_pointer(inst.operands[0].imm.value.u);

  } else {
    cpu.set_inst_pointer(read_operand(inst.operands[0]));
  }

  return true;
}

bool VirtualMachine::neg_(const ZydisDecodedInstruction &inst) {
  write_operand(inst.operands[0], (~read_operand(inst.operands[0]) + 1));
  return true;
}

bool VirtualMachine::ret_(const ZydisDecodedInstruction &inst) {
  const auto address = pop();
  cpu.set_inst_pointer(address);
  return true;
}

bool VirtualMachine::push_(const ZydisDecodedInstruction &inst) {
  const auto value = read_operand(inst.operands[0]);
  push(value);
  return true;
}

bool VirtualMachine::pop_(const ZydisDecodedInstruction &inst) {
  write_operand(inst.operands[0], pop());
  return true;
}

bool VirtualMachine::jmp_(const ZydisDecodedInstruction &inst) {
  auto where = inst.operands[0];
  auto current = cpu.get_inst_pointer();

  if (where.type == ZYDIS_OPERAND_TYPE_IMMEDIATE) {
    if (where.imm.is_relative)
      current += where.imm.value.s;
    else
      current = where.imm.value.u;
  } else {
    current = read_operand(where);
  }
  cpu.set_inst_pointer(current);
  return true;
}

bool VirtualMachine::jz_(const ZydisDecodedInstruction &inst) {
  if (cpu.regs.FLAGS.q & FlagMasks::kZeroFlag)
    return jmp_(inst);
  return true;
}

bool VirtualMachine::jnz_(const ZydisDecodedInstruction &inst) {
  if (!(cpu.regs.FLAGS.q & FlagMasks::kZeroFlag))
    return jmp_(inst);
  return true;
}

bool VirtualMachine::js_(const ZydisDecodedInstruction &inst) {
  if (cpu.regs.FLAGS.q & FlagMasks::kSignFlag)
    return jmp_(inst);
  return true;
}

bool VirtualMachine::jns_(const ZydisDecodedInstruction &inst) {
  if (!(cpu.regs.FLAGS.q & FlagMasks::kSignFlag))
    return jmp_(inst);
  return true;
}

bool VirtualMachine::jp_(const ZydisDecodedInstruction &inst) {
  if (cpu.regs.FLAGS.q & FlagMasks::kParityFlag)
    return jmp_(inst);
  return true;
}

bool VirtualMachine::jnp_(const ZydisDecodedInstruction &inst) {
  if (!(cpu.regs.FLAGS.q & FlagMasks::kParityFlag))
    return jmp_(inst);
  return true;
}

bool VirtualMachine::shr_(const ZydisDecodedInstruction &inst) {
  const auto op1 = read_operand(inst.operands[0]),
             op2 = read_operand(inst.operands[1]);
  write_operand(inst.operands[0], op1 >> op2);

  cpu.regs.FLAGS.q &= ~(FlagMasks::kCarryFlag);

  if ((op1 << (op2 - 1)) & 1)
    cpu.regs.FLAGS.q |= FlagMasks::kCarryFlag;
  return true;
}

bool VirtualMachine::sar_(const ZydisDecodedInstruction &inst) {
  const int64_t op1 = read_operand(inst.operands[0]),
                op2 = read_operand(inst.operands[1]);
  write_operand(inst.operands[0], op1 >> op2);

  cpu.regs.FLAGS.q &= ~(FlagMasks::kCarryFlag);

  if ((op1 << (op2 - 1)) & 1)
    cpu.regs.FLAGS.q |= FlagMasks::kCarryFlag;
  return true;
}

bool VirtualMachine::shl_(const ZydisDecodedInstruction &inst) {
  const auto op1 = read_operand(inst.operands[0]),
             op2 = read_operand(inst.operands[1]);
  write_operand(inst.operands[0], op1 << op2);

  cpu.regs.FLAGS.q &= ~(FlagMasks::kCarryFlag);

  if ((op1 << (op2 - 1ULL)) & (1ULL << inst.operands[0].size))
    cpu.regs.FLAGS.q |= FlagMasks::kCarryFlag;

  return true;
}

bool VirtualMachine::sal_(const ZydisDecodedInstruction &inst) {
  return shl_(inst);
}

bool VirtualMachine::jb_jnae_(const ZydisDecodedInstruction &inst) {
  if (cpu.regs.FLAGS.q & FlagMasks::kCarryFlag)
    return jmp_(inst);
  return true;
}

bool VirtualMachine::jae_jnb_(const ZydisDecodedInstruction &inst) {
  if (!(cpu.regs.FLAGS.q & FlagMasks::kCarryFlag))
    return jmp_(inst);
  return true;
}

bool VirtualMachine::jbe_jna_(const ZydisDecodedInstruction &inst) {
  if (cpu.regs.FLAGS.q & FlagMasks::kCarryFlag &&
      cpu.regs.FLAGS.q & FlagMasks::kZeroFlag)
    return jmp_(inst);
  return true;
}

bool VirtualMachine::ja_jnbe_(const ZydisDecodedInstruction &inst) {
  if (!(cpu.regs.FLAGS.q & FlagMasks::kCarryFlag) &&
      !(cpu.regs.FLAGS.q & FlagMasks::kZeroFlag))
    return jmp_(inst);
  return true;
}

bool VirtualMachine::jl_jnge_(const ZydisDecodedInstruction &inst) {
  if (!!(cpu.regs.FLAGS.q & FlagMasks::kSignFlag) !=
      !!(cpu.regs.FLAGS.q & FlagMasks::kOverflowFlag))
    return jmp_(inst);
  return true;
}

bool VirtualMachine::jge_jnl_(const ZydisDecodedInstruction &inst) {
  if (!!(cpu.regs.FLAGS.q & FlagMasks::kSignFlag) ==
      !!(cpu.regs.FLAGS.q & FlagMasks::kOverflowFlag))
    return jmp_(inst);
  return true;
}

bool VirtualMachine::jle_jng_(const ZydisDecodedInstruction &inst) {
  if (cpu.regs.FLAGS.q & FlagMasks::kZeroFlag &&
      !!(cpu.regs.FLAGS.q & FlagMasks::kSignFlag) !=
          !!(cpu.regs.FLAGS.q & FlagMasks::kOverflowFlag))
    return jmp_(inst);
  return true;
}

bool VirtualMachine::jg_jnle_(const ZydisDecodedInstruction &inst) {
  if (!(cpu.regs.FLAGS.q & FlagMasks::kZeroFlag) &&
      !!(cpu.regs.FLAGS.q & FlagMasks::kSignFlag) ==
          !!(cpu.regs.FLAGS.q & FlagMasks::kOverflowFlag))
    return jmp_(inst);
  return true;
}

bool VirtualMachine::add_(const ZydisDecodedInstruction &inst) {
  const int64_t first_operand = read_operand(inst.operands[0]),
                second_operand = read_operand(inst.operands[1]),
                result = first_operand + second_operand;
  // 64 bit width max
  std::bitset<64> result_bits(result), first_bits(first_operand),
      second_bits(second_operand);

  bool carry = false, adjust = false;

  // do two's complement and get carry flag
  for (size_t i = 0; i < inst.operands[0].size; i++) {
    auto result = carry + first_bits[i] + second_bits[i];
    carry = false;
    if (result & 2)
      carry = true;
    if (i == 4)
      adjust = carry;
  }

  // clear all the flags
  cpu.regs.FLAGS.q &= ~(FlagMasks::kZeroFlag | FlagMasks::kSignFlag |
                        FlagMasks::kCarryFlag | FlagMasks::kOverflowFlag |
                        FlagMasks::kParityFlag | FlagMasks::kAdjustFlag);

  if (result == 0)
    cpu.regs.FLAGS.q |= FlagMasks::kZeroFlag;
  if (result_bits.count() % 2 == 0)
    cpu.regs.FLAGS.q |= FlagMasks::kParityFlag;
  if (result < 0)
    cpu.regs.FLAGS.q |= FlagMasks::kSignFlag;
  if (first_operand > 0 && second_operand > 0 && result < 0)
    cpu.regs.FLAGS.q |= FlagMasks::kOverflowFlag;
  if (carry)
    cpu.regs.FLAGS.q |= FlagMasks::kCarryFlag;
  if (adjust)
    cpu.regs.FLAGS.q |= FlagMasks::kAdjustFlag;

  write_operand(inst.operands[0], result);
  return true;
}

bool VirtualMachine::sub_(const ZydisDecodedInstruction &inst) {
  const int64_t first_operand = read_operand(inst.operands[0]),
                second_operand = read_operand(inst.operands[1]),
                result = first_operand - second_operand;
  // 64 bit width max
  std::bitset<64> result_bits(result), first_bits(first_operand),
      second_bits(~second_operand + 1);

  bool carry = false, adjust = false;

  // do two's complement and get carry flag
  for (size_t i = 0; i < inst.operands[0].size; i++) {
    auto result = carry + first_bits[i] + second_bits[i];
    carry = false;
    if (result & 2)
      carry = true;
    if (i == 4)
      adjust = carry;
  }

  // clear all the flags
  cpu.regs.FLAGS.q &= ~(FlagMasks::kZeroFlag | FlagMasks::kSignFlag |
                        FlagMasks::kCarryFlag | FlagMasks::kOverflowFlag |
                        FlagMasks::kParityFlag | FlagMasks::kAdjustFlag);

  if (result == 0)
    cpu.regs.FLAGS.q |= FlagMasks::kZeroFlag;
  if (result_bits.count() % 2 == 0)
    cpu.regs.FLAGS.q |= FlagMasks::kParityFlag;
  if (result < 0)
    cpu.regs.FLAGS.q |= FlagMasks::kSignFlag;
  if (first_operand > 0 && second_operand > 0 && result < 0)
    cpu.regs.FLAGS.q |= FlagMasks::kOverflowFlag;
  if (carry)
    cpu.regs.FLAGS.q |= FlagMasks::kCarryFlag;
  if (adjust)
    cpu.regs.FLAGS.q |= FlagMasks::kAdjustFlag;

  write_operand(inst.operands[0], result);
  return true;
}

bool VirtualMachine::xor_(const ZydisDecodedInstruction &inst) {
  const auto first_operand = read_operand(inst.operands[0]),
             second_operand = read_operand(inst.operands[1]),
             result = first_operand ^ second_operand;

  cpu.regs.FLAGS.q &=
      ~(FlagMasks::kZeroFlag | FlagMasks::kSignFlag | FlagMasks::kCarryFlag |
        FlagMasks::kOverflowFlag | FlagMasks::kParityFlag);

  std::bitset<64> result_bits(result);
  if ((int64_t)result < 0)
    cpu.regs.FLAGS.q |= FlagMasks::kSignFlag;
  if (result == 0)
    cpu.regs.FLAGS.q |= FlagMasks::kZeroFlag;
  if (result_bits.count() % 2 == 0)
    cpu.regs.FLAGS.q |= FlagMasks::kParityFlag;

  write_operand(inst.operands[0], result);
  return true;
}

bool VirtualMachine::and_(const ZydisDecodedInstruction &inst) {
  const auto first_operand = read_operand(inst.operands[0]),
             second_operand = read_operand(inst.operands[1]),
             result = first_operand & second_operand;

  cpu.regs.FLAGS.q &=
      ~(FlagMasks::kZeroFlag | FlagMasks::kSignFlag | FlagMasks::kCarryFlag |
        FlagMasks::kOverflowFlag | FlagMasks::kParityFlag);

  std::bitset<64> result_bits(result);
  if ((int64_t)result < 0)
    cpu.regs.FLAGS.q |= FlagMasks::kSignFlag;
  if (result == 0)
    cpu.regs.FLAGS.q |= FlagMasks::kZeroFlag;
  if (result_bits.count() % 2 == 0)
    cpu.regs.FLAGS.q |= FlagMasks::kParityFlag;

  write_operand(inst.operands[0], result);
  return true;
}

bool VirtualMachine::or_(const ZydisDecodedInstruction &inst) {
  const auto first_operand = read_operand(inst.operands[0]),
             second_operand = read_operand(inst.operands[1]),
             result = first_operand | second_operand;

  cpu.regs.FLAGS.q &=
      ~(FlagMasks::kZeroFlag | FlagMasks::kSignFlag | FlagMasks::kCarryFlag |
        FlagMasks::kOverflowFlag | FlagMasks::kParityFlag);

  std::bitset<64> result_bits(result);
  if ((int64_t)result < 0)
    cpu.regs.FLAGS.q |= FlagMasks::kSignFlag;
  if (result == 0)
    cpu.regs.FLAGS.q |= FlagMasks::kZeroFlag;
  if (result_bits.count() % 2 == 0)
    cpu.regs.FLAGS.q |= FlagMasks::kParityFlag;

  write_operand(inst.operands[0], result);
  return true;
}

bool VirtualMachine::test_(const ZydisDecodedInstruction &inst) {
  const auto first_operand = read_operand(inst.operands[0]),
             second_operand = read_operand(inst.operands[1]),
             result = first_operand & second_operand;
  // 64 bit width max
  std::bitset<64> result_bits(result), first_bits(first_operand),
      second_bits(~second_operand + 1);

  // clear all the flags
  cpu.regs.FLAGS.q &=
      ~(FlagMasks::kZeroFlag | FlagMasks::kSignFlag | FlagMasks::kCarryFlag |
        FlagMasks::kOverflowFlag | FlagMasks::kParityFlag);

  if (result == 0)
    cpu.regs.FLAGS.q |= FlagMasks::kZeroFlag;
  if (result_bits.count() % 2 == 0)
    cpu.regs.FLAGS.q |= FlagMasks::kParityFlag;
  if (result < 0)
    cpu.regs.FLAGS.q |= FlagMasks::kSignFlag;

  return true;
}

bool VirtualMachine::cmp_(const ZydisDecodedInstruction &inst) {
  const auto first_operand = read_operand(inst.operands[0]),
             second_operand = read_operand(inst.operands[1]),
             result = first_operand - second_operand;
  // 64 bit width max
  std::bitset<64> result_bits(result), first_bits(first_operand),
      second_bits(~second_operand + 1);

  bool carry = false, adjust = false;

  // do two's complement and get carry flag
  for (size_t i = 0; i < inst.operands[0].size; i++) {
    auto result = carry + first_bits[i] + second_bits[i];
    carry = false;
    if (result & 2)
      carry = true;
    if (i == 4)
      adjust = carry;
  }

  // clear all the flags
  cpu.regs.FLAGS.q &= ~(FlagMasks::kZeroFlag | FlagMasks::kSignFlag |
                        FlagMasks::kCarryFlag | FlagMasks::kOverflowFlag |
                        FlagMasks::kParityFlag | FlagMasks::kAdjustFlag);

  if (result == 0)
    cpu.regs.FLAGS.q |= FlagMasks::kZeroFlag;
  if (result_bits.count() % 2 == 0)
    cpu.regs.FLAGS.q |= FlagMasks::kParityFlag;
  if (result < 0)
    cpu.regs.FLAGS.q |= FlagMasks::kSignFlag;
  if (first_operand > 0 && second_operand > 0 && result < 0)
    cpu.regs.FLAGS.q |= FlagMasks::kOverflowFlag;
  if (carry)
    cpu.regs.FLAGS.q |= FlagMasks::kCarryFlag;
  if (adjust)
    cpu.regs.FLAGS.q |= FlagMasks::kAdjustFlag;

  return true;
}

bool VirtualMachine::syscall(const ZydisDecodedInstruction &inst) {
  // core::kernel::syscalls::impl_syscall(cpu);
  return true;
}

bool VirtualMachine::recompile(const ZydisDecodedInstruction &inst) {
  return false;
}

VirtualMachine::VirtualMachine(VirtualCpu &cpu_, kernel::Kernel &krnl_,
                               memory::MemoryController &mcontrol_,
                               Emulator *emul_)
    : cpu(cpu_), mcontrol(mcontrol_), inst_mappings{}, emulator(emul_),
      krnl(krnl_) {
  inst_mappings[ZYDIS_MNEMONIC_ADD] = &VirtualMachine::add_;

  inst_mappings[ZYDIS_MNEMONIC_PUSH] = &VirtualMachine::push_;
  inst_mappings[ZYDIS_MNEMONIC_POP] = &VirtualMachine::pop_;

  inst_mappings[ZYDIS_MNEMONIC_MOV] = &VirtualMachine::mov_;
  inst_mappings[ZYDIS_MNEMONIC_SUB] = &VirtualMachine::sub_;

  inst_mappings[ZYDIS_MNEMONIC_LEA] = &VirtualMachine::lea_;

  inst_mappings[ZYDIS_MNEMONIC_NEG] = &VirtualMachine::neg_;
  inst_mappings[ZYDIS_MNEMONIC_CMP] = &VirtualMachine::cmp_;
  inst_mappings[ZYDIS_MNEMONIC_TEST] = &VirtualMachine::test_;

  inst_mappings[ZYDIS_MNEMONIC_XOR] = &VirtualMachine::xor_;
  inst_mappings[ZYDIS_MNEMONIC_OR] = &VirtualMachine::or_;
  inst_mappings[ZYDIS_MNEMONIC_AND] = &VirtualMachine::and_;

  inst_mappings[ZYDIS_MNEMONIC_JZ] = &VirtualMachine::jz_;
  inst_mappings[ZYDIS_MNEMONIC_JNZ] = &VirtualMachine::jnz_;
  inst_mappings[ZYDIS_MNEMONIC_JS] = &VirtualMachine::js_;
  inst_mappings[ZYDIS_MNEMONIC_JNS] = &VirtualMachine::jns_;
  inst_mappings[ZYDIS_MNEMONIC_JP] = &VirtualMachine::jp_;
  inst_mappings[ZYDIS_MNEMONIC_JNP] = &VirtualMachine::jnp_;

  inst_mappings[ZYDIS_MNEMONIC_JB] = &VirtualMachine::jb_jnae_;
  inst_mappings[ZYDIS_MNEMONIC_JNB] = &VirtualMachine::jae_jnb_;
  inst_mappings[ZYDIS_MNEMONIC_JBE] = &VirtualMachine::jbe_jna_;
  inst_mappings[ZYDIS_MNEMONIC_JNBE] = &VirtualMachine::ja_jnbe_;

  inst_mappings[ZYDIS_MNEMONIC_SHR] = &VirtualMachine::shr_;
  inst_mappings[ZYDIS_MNEMONIC_SHL] = &VirtualMachine::shl_;
  inst_mappings[ZYDIS_MNEMONIC_SAR] = &VirtualMachine::sar_;
  // inst_mappings[ZYDIS_MNEMONIC_SAL] = &VirtualMachine::sal_;

  inst_mappings[ZYDIS_MNEMONIC_JL] = &VirtualMachine::jl_jnge_;
  inst_mappings[ZYDIS_MNEMONIC_JNL] = &VirtualMachine::jge_jnl_;
  inst_mappings[ZYDIS_MNEMONIC_JLE] = &VirtualMachine::jle_jng_;
  inst_mappings[ZYDIS_MNEMONIC_JNLE] = &VirtualMachine::jg_jnle_;

  inst_mappings[ZYDIS_MNEMONIC_INT] = &VirtualMachine::int_;
  inst_mappings[ZYDIS_MNEMONIC_SYSCALL] = &VirtualMachine::syscall;
  inst_mappings[ZYDIS_MNEMONIC_RET] = &VirtualMachine::ret_;
  inst_mappings[ZYDIS_MNEMONIC_CALL] = &VirtualMachine::call_;
  inst_mappings[ZYDIS_MNEMONIC_JMP] = &VirtualMachine::jmp_;
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
