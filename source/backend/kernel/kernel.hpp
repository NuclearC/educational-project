#ifndef EMUCORE_KERNELIMPL_SYSCALLS_HPP_
#define EMUCORE_KERNELIMPL_SYSCALLS_HPP_

#include <iostream>

#include "backend/memory/memory.hpp"
#include "backend/cpu/cpu.hpp"

namespace core {
namespace kernel {
class Kernel {
private:
  core::backend::memory::MemoryController& mcontrol;
  core::backend::cpu::VirtualCpu &cpu;

public:
  Kernel(core::backend::memory::MemoryController &mcontrol_,
         core::backend::cpu::VirtualCpu &cpu_);
  ~Kernel();

  void reset();

  void impl_sys_restart_syscall_();

  void impl_sys_exit_(int error_code);
  void impl_sys_fork_(void *pt_regs);

  void impl_sys_read_(unsigned int fd, char *buf, std::size_t count);
  void impl_sys_write_(unsigned int fd, const char* buf, std::size_t count);
  void impl_sys_open_(const char* filename, int flags, int mode);
  void impl_sys_close_(unsigned int fd);
  void impl_sys_waitpid_(uint32_t pid, int *stat_addr, int options);
  void impl_sys_creat_(const char* pathname, int mode);
  void impl_sys_link_(const char* oldname, const char* newname);
};

} // namespace kernel
} // namespace core

#endif // EMUCORE_KERNELIMPL_SYSCALLS_HPP_
