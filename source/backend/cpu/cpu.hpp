#ifndef CPU_HPP_
#define CPU_HPP_

#include <stack>

#include "backend/cpu/registers.hpp"
#include "backend/memory/memory.hpp"

namespace core {
namespace backend {
namespace cpu {
class VirtualCpu {
  friend class VirtualMachine;

private:
  registers::Registers regs;
  // std::stack<uint64_t> stack;

  memory::MemoryController &mcontrol;
public:
  VirtualCpu(memory::MemoryController & _control);
  ~VirtualCpu();

  // get pointer to the program
  char *fetch();

  void set_inst_pointer(uint64_t val);
  void set_stack_pointer(uint64_t val);
  void set_base_pointer(uint64_t val);
  uint64_t get_inst_pointer();
  void reset();
  void poll();
};
} // namespace cpu
} // namespace backend
} // namespace core

#endif // CPU_HPP_

