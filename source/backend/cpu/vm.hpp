#ifndef VM_HPP_
#define VM_HPP_

#include <memory>
#include <bitset>
#include <variant>

#include "backend/cpu/cpu.hpp"
#include "backend/memory/memory.hpp"

#include <Zydis/Zydis.h>

#include <asmjit/asmjit.h>

namespace core {

  class Emulator;

namespace backend {
namespace cpu {

typedef uint64_t (*AsmjitFunc)();

class VirtualMachine {
private:
  asmjit::CodeHolder code_holder;
  asmjit::JitRuntime jit_runtime;
  std::unique_ptr<asmjit::X86Assembler> a;
  Emulator * emulator;

  VirtualCpu &cpu;
  memory::MemoryController &mcontrol;

  uint64_t get_operand_value(const ZydisDecodedOperand &operand);
  void write_operand_value(const ZydisDecodedOperand &operand, uint64_t value);

  bool mov_(const ZydisDecodedInstruction &inst);
  bool add_(const ZydisDecodedInstruction &inst);
  bool sub_(const ZydisDecodedInstruction &inst);
  bool xor_(const ZydisDecodedInstruction &inst);
  bool ret_(const ZydisDecodedInstruction &inst);
  bool push_(const ZydisDecodedInstruction &inst);
  bool pop_(const ZydisDecodedInstruction &inst);
  bool and_(const ZydisDecodedInstruction &inst);
  bool or_(const ZydisDecodedInstruction &inst);
  bool syscall(const ZydisDecodedInstruction &inst);

  bool recompile(const ZydisDecodedInstruction &inst);

  // initialize asmjit for executing commands
  void asmjit_begin();
  // execute the command list and return the value
  uint64_t asmjit_end();

  typedef bool (VirtualMachine::*InstructionMapping)(
      const ZydisDecodedInstruction &);
  InstructionMapping inst_mappings[1582];

public:
  VirtualMachine(VirtualCpu &cpu_, memory::MemoryController &mcontrol_,
    Emulator* emul_);
  ~VirtualMachine();

  bool execute_instruction(const ZydisDecodedInstruction &inst);
};
} // namespace cpu
} // namespace backend
} // namespace core

#endif // VM_HPP_
