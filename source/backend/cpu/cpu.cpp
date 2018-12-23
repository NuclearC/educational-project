#include "backend/cpu/cpu.hpp"
#include "cpu.hpp"

namespace core {
namespace backend {
namespace cpu {

VirtualCpu::VirtualCpu(memory::MemoryController &_mcontrol)
    : mcontrol(_mcontrol) {
  reset();
}
VirtualCpu::~VirtualCpu() {}

char *VirtualCpu::fetch() { return (char *)mcontrol[regs.RIP.q]; }

void VirtualCpu::set_inst_pointer(uint64_t val) { regs.RIP.q = val; }

void VirtualCpu::set_stack_pointer(uint64_t val) { regs.RSP.q = val; }

void VirtualCpu::set_base_pointer(uint64_t val) { regs.RBP.q = val; }

uint64_t VirtualCpu::get_inst_pointer() { return regs.RIP.q; }

void VirtualCpu::reset() { 
  regs = {};
}

void VirtualCpu::poll() {
  
}

} // namespace cpu
} // namespace backend
} // namespace core
