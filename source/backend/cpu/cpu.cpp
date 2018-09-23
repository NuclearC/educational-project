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

char *VirtualCpu::fetch() { return (char*)mcontrol[regs.rip]; }

void VirtualCpu::set_inst_pointer(uint64_t val) { regs.rip = val; }

uint64_t VirtualCpu::get_inst_pointer() { return regs.rip; }

void VirtualCpu::reset() { 
  stack = std::stack<uint64_t>();
  regs = {};
}

void VirtualCpu::poll() {
  
}

} // namespace cpu
} // namespace backend
} // namespace core
