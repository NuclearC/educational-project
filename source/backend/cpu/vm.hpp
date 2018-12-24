#ifndef VM_HPP_
#define VM_HPP_

#include <bitset>
#include <memory>
#include <variant>

#include "backend/cpu/cpu.hpp"
#include "backend/kernel/kernel.hpp"
#include "backend/memory/memory.hpp"

#include <Zydis/Zydis.h>
#include <asmjit/asmjit.h>

namespace core {

class Emulator;

namespace backend {
namespace cpu {

using namespace registers;

typedef uint64_t (*AsmjitFunc)();

class VirtualMachine {
private:
  // asmjit stuff
  asmjit::JitRuntime rt;

  Emulator *emulator;

  VirtualCpu &cpu;
  kernel::Kernel &krnl;
  memory::MemoryController &mcontrol;

  // uint64_t get_operand_value(const ZydisDecodedOperand &operand);
  // void write_operand_value(const ZydisDecodedOperand &operand, uint64_t
  // value);

  void handle_syscall(int op);

  uint64_t read_operand(const ZydisDecodedOperand &operand);
  void write_operand(const ZydisDecodedOperand &operand, uint64_t value);

  void push(uint64_t value);
  uint64_t pop();

  bool int_(const ZydisDecodedInstruction &inst);
  bool mov_(const ZydisDecodedInstruction &inst);
  bool lea_(const ZydisDecodedInstruction &inst);

  // arithmetic
  bool add_(const ZydisDecodedInstruction &inst);
  bool sub_(const ZydisDecodedInstruction &inst);

  // bitwise
  bool xor_(const ZydisDecodedInstruction &inst);
  bool and_(const ZydisDecodedInstruction &inst);
  bool or_(const ZydisDecodedInstruction &inst);

  bool push_(const ZydisDecodedInstruction &inst);
  bool pop_(const ZydisDecodedInstruction &inst);

  bool test_(const ZydisDecodedInstruction &inst);
  bool cmp_(const ZydisDecodedInstruction &inst);
  bool syscall(const ZydisDecodedInstruction &inst);

  bool call_(const ZydisDecodedInstruction &inst);
  bool jmp_(const ZydisDecodedInstruction &inst);

  bool jz_(const ZydisDecodedInstruction &inst);
  bool jnz_(const ZydisDecodedInstruction &inst);
  bool js_(const ZydisDecodedInstruction &inst);
  bool jns_(const ZydisDecodedInstruction &inst);
  bool jp_(const ZydisDecodedInstruction &inst);
  bool jnp_(const ZydisDecodedInstruction &inst);

  bool shr_(const ZydisDecodedInstruction &inst);
  bool sar_(const ZydisDecodedInstruction &inst);
  bool shl_(const ZydisDecodedInstruction &inst);
  bool sal_(const ZydisDecodedInstruction &inst);

  // unsigned comparisions
  bool jb_jnae_(const ZydisDecodedInstruction &inst);
  bool jae_jnb_(const ZydisDecodedInstruction &inst);
  bool jbe_jna_(const ZydisDecodedInstruction &inst);
  bool ja_jnbe_(const ZydisDecodedInstruction &inst);

  // signed comparisions
  bool jl_jnge_(const ZydisDecodedInstruction &inst);
  bool jge_jnl_(const ZydisDecodedInstruction &inst);
  bool jle_jng_(const ZydisDecodedInstruction &inst);
  bool jg_jnle_(const ZydisDecodedInstruction &inst);

  bool neg_(const ZydisDecodedInstruction &inst);
  bool ret_(const ZydisDecodedInstruction &inst);

  bool recompile(const ZydisDecodedInstruction &inst);

  typedef bool (VirtualMachine::*InstructionMapping)(
      const ZydisDecodedInstruction &);
  InstructionMapping inst_mappings[1582];

public:
  VirtualMachine(VirtualCpu &cpu_, kernel::Kernel &krnl_,
                 memory::MemoryController &mcontrol_, Emulator *emul_);
  ~VirtualMachine();

  bool execute_instruction(const ZydisDecodedInstruction &inst);
};
} // namespace cpu
} // namespace backend
} // namespace core

#endif // VM_HPP_
