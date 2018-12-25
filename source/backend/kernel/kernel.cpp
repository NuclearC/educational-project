
#include "backend/kernel/kernel.hpp"
#include "backend/cpu/cpu.hpp"

namespace core {
namespace kernel {

using namespace core::utils;

void Kernel::close_all_files() {
  for (auto i = MAX_FILES_OPEN; i > 0; i--) {
    if (files[i] != nullptr)
      close_file(i);
  }
}

void Kernel::close_file(std::size_t index) {
  if (files[index] != nullptr && files[index]->get_state() == kOpen)
    files[index]->close();
}

void Kernel::open_stdio() { 
  files[1] = std::make_unique<FileDescriptor>();
  files[1]->open("1", 0, 0);
}

void Kernel::write_return(int i) {}

Kernel::Kernel(core::backend::memory::MemoryController &mcontrol_,
               core::backend::cpu::VirtualCpu &cpu_)
    : mcontrol(mcontrol_), cpu(cpu_) {}

Kernel::~Kernel() {}

void Kernel::reset() { open_stdio(); }

void Kernel::impl_sys_restart_syscall_() {
  log("SYSCALL impl_sys_restart_syscall_", kInfo);
}

void Kernel::impl_sys_exit_(int error_code) {
  log("SYSCALL impl_sys_exit_", kInfo);
  log("Program exited with code: " + std::to_string(error_code), kInfo);

  close_all_files();

  cpu.break_();
}

void Kernel::impl_sys_fork_(void *pt_regs) {
  log("SYSCALL impl_sys_fork_", kInfo);
}

void Kernel::impl_sys_read_(unsigned int fd, char *buf, std::size_t count) {
  log("SYSCALL impl_sys_read_", kInfo);

  if (files[fd] == nullptr || files[fd]->get_state() == kClosed)
    throw std::exception("attemt of reading closed file");

  files[fd]->read(mcontrol[(uint64_t)buf], count);
}

void Kernel::impl_sys_write_(unsigned int fd, const char *buf,
                             std::size_t count) {
  log("SYSCALL impl_sys_write_", kInfo);

  if (files[fd] == nullptr || files[fd]->get_state() == kClosed)
    throw std::exception("attemt of writing closed file");

  files[fd]->write(mcontrol[(uint64_t)buf], count);
}

void Kernel::impl_sys_open_(const char *filename, int flags, int mode) {
  log("SYSCALL impl_sys_open_", kInfo);

  for (auto i = MAX_FILES_OPEN; i > 0; i--) {
    if (files[i] == nullptr || files[i]->get_state() == kClosed) {
      files[i] = std::make_unique<FileDescriptor>();
      files[i]->open(filename, flags, mode);
      write_return(i);
      break;
    }
  }
}

void Kernel::impl_sys_close_(unsigned int fd) {
  log("SYSCALL impl_sys_close_", kInfo);

  if (files[fd] == nullptr || files[fd]->get_state() == kClosed)
    throw std::exception("closing already closed file");

  files[fd]->close();
}

void Kernel::impl_sys_waitpid_(uint32_t pid, int *stat_addr, int options) {
  log("SYSCALL impl_sys_waitpid_", kInfo);
}

void Kernel::impl_sys_creat_(const char *pathname, int mode) {
  log("SYSCALL impl_sys_creat_", kInfo);
}

void Kernel::impl_sys_link_(const char *oldname, const char *newname) {
  log("SYSCALL impl_sys_link_", kInfo);
}

FileDescriptor::FileDescriptor() {}

FileDescriptor::~FileDescriptor() {}

void FileDescriptor::open(std::string_view filename, int flags, int mode) {
  write = [](const void *data, std::size_t len) {
    std::cout << std::string((const char *)data, len);
  };
  read = nullptr;
}

void FileDescriptor::close() {}

FileDescriptorState FileDescriptor::get_state() const { return state; }

} // namespace kernel
} // namespace core
  