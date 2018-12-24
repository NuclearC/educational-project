
#include "backend/kernel/kernel.hpp"
#include "backend/cpu/cpu.hpp"

namespace core {
namespace kernel {

Kernel::Kernel(core::backend::memory::MemoryController &mcontrol_,
               core::backend::cpu::VirtualCpu &cpu_)
    : mcontrol(mcontrol_), cpu(cpu_) {}

Kernel::~Kernel() {}

void Kernel::reset() {}

void Kernel::impl_sys_restart_syscall_() {}

void Kernel::impl_sys_exit_(int error_code) {}

void Kernel::impl_sys_fork_(void *pt_regs) {}

void Kernel::impl_sys_read_(unsigned int fd, char *buf, std::size_t count) {}

void Kernel::impl_sys_write_(unsigned int fd, const char *buf,
                             std::size_t count) {

}

void Kernel::impl_sys_open_(const char *filename, int flags, int mode) {}

void Kernel::impl_sys_close_(unsigned int fd) {}

void Kernel::impl_sys_waitpid_(uint32_t pid, int *stat_addr, int options) {}

void Kernel::impl_sys_creat_(const char *pathname, int mode) {}

void Kernel::impl_sys_link_(const char *oldname, const char *newname) {}

} // namespace kernel
} // namespace core
