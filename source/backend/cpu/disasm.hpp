#ifndef DISASM_HPP_
#define DISASM_HPP_

#include <iostream>
#include <iomanip>

#include <Zydis/Zydis.h>

#include "backend/cpu/cpu.hpp"
#include "backend/cpu/vm.hpp"

namespace core {
namespace backend {
namespace cpu {

struct InstructionInfo {
  uint64_t instruction_pointer;
  ZydisDecodedInstruction instruction;
};

class DisAsm {
private:
  ZydisDecoder decoder;
  ZydisFormatter formatter;
  VirtualCpu &cpu;
  VirtualMachine &vm;

public:
  DisAsm(VirtualCpu &cpu_, VirtualMachine& vm_);
  ~DisAsm();

  InstructionInfo try_decode(uint64_t pointer, const void *data,
                                    size_t length);
  void print(const InstructionInfo& info);
  bool try_execute(const InstructionInfo &info);

  void poll();
};
} // namespace cpu
} // namespace backend
} // namespace core

#endif // DISASM_HPP_
