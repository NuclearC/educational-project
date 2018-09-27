#include "backend/cpu/vm.hpp"
#include "emulator.hpp"

namespace core {
namespace backend {
namespace cpu {

uint64_t VirtualMachine::get_operand_value(const ZydisDecodedOperand &operand) {
  switch (operand.type) {
  case ZYDIS_OPERAND_TYPE_IMMEDIATE:
    return operand.imm.value.u;
  // case ZYDIS_OPERAND_TYPE_MEMORY:
  //  return 0;
  case ZYDIS_OPERAND_TYPE_REGISTER:
    return cpu.regs.read_general(
        registers::ZydisRegisterIndices[operand.reg.value]);
  // case ZYDIS_OPERAND_TYPE_POINTER:
  //  return 0;
  default:
    _CrtDbgBreak();
  }
}

void VirtualMachine::write_operand_value(const ZydisDecodedOperand &operand,
                                         uint64_t value) {
  switch (operand.type) {
  // case ZYDIS_OPERAND_TYPE_IMMEDIATE:
  //  return operand.imm.value.u;
  // case ZYDIS_OPERAND_TYPE_MEMORY:
  //  return 0;
  case ZYDIS_OPERAND_TYPE_REGISTER:
    return cpu.regs.write_general(
        registers::ZydisRegisterIndices[operand.reg.value], value);
  // case ZYDIS_OPERAND_TYPE_POINTER:
  //  return 0;
  default:
    _CrtDbgBreak();
  }
}

bool VirtualMachine::mov_(const ZydisDecodedInstruction &inst) {
  if (inst.operandCount != 2)
    return false;
  uint64_t value = get_operand_value(inst.operands[1]);
  write_operand_value(inst.operands[0], value);
  return true;
}

bool VirtualMachine::add_(const ZydisDecodedInstruction &inst) {
  if (inst.operandCount != 3)
    return false;
  const auto value = get_operand_value(inst.operands[0]),
             value_to_add = get_operand_value(inst.operands[1]);
  write_operand_value(inst.operands[0], value + value_to_add);
  return true;
}

bool VirtualMachine::ret_(const ZydisDecodedInstruction &inst) {
  // hang the emulator
  emulator->state(true);
  return true;
}

bool VirtualMachine::push_(const ZydisDecodedInstruction &inst) {
  const auto value = get_operand_value(inst.operands[0]);
  cpu.stack.push(value);
  return true;
}

bool VirtualMachine::pop_(const ZydisDecodedInstruction &inst) {
  write_operand_value(inst.operands[0], cpu.stack.top());
  cpu.stack.pop();
  return true;
}

bool VirtualMachine::syscall(const ZydisDecodedInstruction &inst) {
  switch (cpu.regs.read_general(registers::rax)) {
  case 1: {
    const auto count = cpu.regs.read_general(registers::rcx);
    const auto data = cpu.regs.read_general(registers::rdx);

    std::cout << std::string((const char *)mcontrol[data], count) << std::endl;
    return true;
  } break;
  }

  return false;
}

bool VirtualMachine::recompile(const ZydisDecodedInstruction &inst) {
  return false;
}

void VirtualMachine::asmjit_begin() {
  a = std::make_unique<asmjit::X86Assembler>(&code_holder);
}

uint64_t VirtualMachine::asmjit_end() {
  AsmjitFunc func = nullptr;
  auto err = jit_runtime.add(&func, &code_holder);
  if (err) {
    throw std::exception("failed to execute assembly command list");
  }
  return func();
}

VirtualMachine::VirtualMachine(VirtualCpu &cpu_,
                               memory::MemoryController &mcontrol_,
                               Emulator *emul_)
    : cpu(cpu_), mcontrol(mcontrol_), inst_mappings{}, emulator(emul_) {
  inst_mappings[ZYDIS_MNEMONIC_ADD] = &VirtualMachine::add_;
  inst_mappings[ZYDIS_MNEMONIC_MOV] = &VirtualMachine::mov_;
  inst_mappings[ZYDIS_MNEMONIC_SYSCALL] = &VirtualMachine::syscall;
  inst_mappings[ZYDIS_MNEMONIC_RET] = &VirtualMachine::ret_;
  inst_mappings[ZYDIS_MNEMONIC_PUSH] = &VirtualMachine::push_;
  inst_mappings[ZYDIS_MNEMONIC_POP] = &VirtualMachine::pop_;

  // initialize asmjit
  code_holder.init(jit_runtime.getCodeInfo());
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
