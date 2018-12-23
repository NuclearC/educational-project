#include "backend/cpu/registers.hpp"

namespace core {
namespace backend {
namespace cpu {
namespace registers {

uint64_t Registers::read(ZydisRegister reg) { 
  switch (reg) { 
  case 0:
    return 0;
    // general purpose 4 regs
  case ZYDIS_REGISTER_AH:
    return RAX.w & 0xff00;
  case ZYDIS_REGISTER_AL:
    return RAX.l;
  case ZYDIS_REGISTER_AX:
    return RAX.w;
  case ZYDIS_REGISTER_EAX:
    return RAX.d;
  case ZYDIS_REGISTER_RAX:
    return RAX.q;
  case ZYDIS_REGISTER_BH:
    return RBX.w & 0xff00;
  case ZYDIS_REGISTER_BL:
    return RBX.l;
  case ZYDIS_REGISTER_BX:
    return RBX.w;
  case ZYDIS_REGISTER_EBX:
    return RBX.d;
  case ZYDIS_REGISTER_RBX:
    return RBX.q;
  case ZYDIS_REGISTER_CH:
    return RCX.w & 0xff00;
  case ZYDIS_REGISTER_CL:
    return RCX.l;
  case ZYDIS_REGISTER_CX:
    return RCX.w;
  case ZYDIS_REGISTER_ECX:
    return RCX.d;
  case ZYDIS_REGISTER_RCX:
    return RCX.q;
  case ZYDIS_REGISTER_DH:
    return RDX.w & 0xff00;
  case ZYDIS_REGISTER_DL:
    return RDX.l;
  case ZYDIS_REGISTER_DX:
    return RDX.w;
  case ZYDIS_REGISTER_EDX:
    return RDX.d;
  case ZYDIS_REGISTER_RDX:
    return RDX.q;
    // stack control registers
  case ZYDIS_REGISTER_SP:
    return RSP.w;
  case ZYDIS_REGISTER_ESP:
    return RSP.d;
  case ZYDIS_REGISTER_RSP:
    return RSP.q;
  case ZYDIS_REGISTER_BP:
    return RBP.w;
  case ZYDIS_REGISTER_EBP:
    return RBP.d;
  case ZYDIS_REGISTER_RBP:
    return RBP.q;
    // index control registers
  case ZYDIS_REGISTER_DI:
    return RDI.w;
  case ZYDIS_REGISTER_EDI:
    return RDI.d;
  case ZYDIS_REGISTER_RDI:
    return RDI.q;
  case ZYDIS_REGISTER_SI:
    return RSI.w;
  case ZYDIS_REGISTER_ESI:
    return RSI.d;
  case ZYDIS_REGISTER_RSI:
    return RSI.q;
    // other general purpose registers
  case ZYDIS_REGISTER_R8B:
    return R8.l;
  case ZYDIS_REGISTER_R8W:
    return R8.w;
  case ZYDIS_REGISTER_R8D:
    return R8.d;
  case ZYDIS_REGISTER_R8:
    return R8.q;
  case ZYDIS_REGISTER_R9B:
    return R9.l;
  case ZYDIS_REGISTER_R9W:
    return R9.w;
  case ZYDIS_REGISTER_R9D:
    return R9.d;
  case ZYDIS_REGISTER_R9:
    return R9.q;
  case ZYDIS_REGISTER_R10B:
    return R10.l;
  case ZYDIS_REGISTER_R10W:
    return R10.w;
  case ZYDIS_REGISTER_R10D:
    return R10.d;
  case ZYDIS_REGISTER_R10:
    return R10.q;
  case ZYDIS_REGISTER_R11B:
    return R11.l;
  case ZYDIS_REGISTER_R11W:
    return R11.w;
  case ZYDIS_REGISTER_R11D:
    return R11.d;
  case ZYDIS_REGISTER_R11:
    return R11.q;
  case ZYDIS_REGISTER_R12B:
    return R12.l;
  case ZYDIS_REGISTER_R12W:
    return R12.w;
  case ZYDIS_REGISTER_R12D:
    return R12.d;
  case ZYDIS_REGISTER_R12:
    return R12.q;
  case ZYDIS_REGISTER_R13B:
    return R13.l;
  case ZYDIS_REGISTER_R13W:
    return R13.w;
  case ZYDIS_REGISTER_R13D:
    return R13.d;
  case ZYDIS_REGISTER_R13:
    return R13.q;
  case ZYDIS_REGISTER_R14B:
    return R14.l;
  case ZYDIS_REGISTER_R14W:
    return R14.w;
  case ZYDIS_REGISTER_R14D:
    return R14.d;
  case ZYDIS_REGISTER_R14:
    return R14.q;
  case ZYDIS_REGISTER_R15B:
    return R15.l;
  case ZYDIS_REGISTER_R15W:
    return R15.w;
  case ZYDIS_REGISTER_R15D:
    return R15.d;
  case ZYDIS_REGISTER_R15:
    return R15.q;
  case ZYDIS_REGISTER_IP:
    return RIP.w;
  case ZYDIS_REGISTER_EIP:
    return RIP.d;
  case ZYDIS_REGISTER_RIP:
    return RIP.q;
  case ZYDIS_REGISTER_FLAGS:
    return FLAGS.w;
  case ZYDIS_REGISTER_EFLAGS:
    return FLAGS.d;
  case ZYDIS_REGISTER_RFLAGS:
    return FLAGS.q;
  case ZYDIS_REGISTER_ES:
  case ZYDIS_REGISTER_CS:
  case ZYDIS_REGISTER_SS:
  case ZYDIS_REGISTER_DS:
  case ZYDIS_REGISTER_FS:
  case ZYDIS_REGISTER_GS:
    throw std::exception("segment register not supported");

  default:
    throw std::exception("unrealized register");
  }
}

void Registers::write(ZydisRegister reg, uint64_t val) { 
  switch (reg) {
    // general purpose 4 regs
  case ZYDIS_REGISTER_AH:
    RAX.w = val & 0xff00;
    break;
  case ZYDIS_REGISTER_AL:
    RAX.l = val & 0xff;
    break;
  case ZYDIS_REGISTER_AX:
    RAX.w = val & 0xffff;
    break;
  case ZYDIS_REGISTER_EAX:
    RAX.d = val & 0xffffffff;
    break;
  case ZYDIS_REGISTER_RAX:
    RAX.q = val;
    break;
  case ZYDIS_REGISTER_BH:
    RBX.w = val & 0xff00;
    break;
  case ZYDIS_REGISTER_BL:
    RBX.l = val & 0xff;
    break;
  case ZYDIS_REGISTER_BX:
    RBX.w = val & 0xffff;
    break;
  case ZYDIS_REGISTER_EBX:
    RBX.d = val & 0xffffffff;
    break;
  case ZYDIS_REGISTER_RBX:
    RBX.q = val;
    break;
  case ZYDIS_REGISTER_CH:
    RCX.w = val & 0xff00;
    break;
  case ZYDIS_REGISTER_CL:
    RCX.l = val & 0xff;
    break;
  case ZYDIS_REGISTER_CX:
    RCX.w = val & 0xffff;
    break;
  case ZYDIS_REGISTER_ECX:
    RCX.d = val & 0xffffffff;
    break;
  case ZYDIS_REGISTER_RCX:
    RCX.q = val;
    break;
  case ZYDIS_REGISTER_DH:
    RDX.w = val & 0xff00;
    break;
  case ZYDIS_REGISTER_DL:
    RDX.l = val & 0xff;
    break;
  case ZYDIS_REGISTER_DX:
    RDX.w = val & 0xffff;
    break;
  case ZYDIS_REGISTER_EDX:
    RDX.d = val & 0xffffffff;
    break;
  case ZYDIS_REGISTER_RDX:
    RDX.q = val;
    break;
    // stack control registers
  case ZYDIS_REGISTER_SP:
    RSP.w = val & 0xffff;
    break;
  case ZYDIS_REGISTER_ESP:
    RSP.d = val & 0xffffffff;
    break;
  case ZYDIS_REGISTER_RSP:
    RSP.q = val;
    break;
  case ZYDIS_REGISTER_BP:
    RBP.w = val & 0xffff;
    break;
  case ZYDIS_REGISTER_EBP:
    RBP.d = val & 0xffffffff;
    break;
  case ZYDIS_REGISTER_RBP:
    RBP.q = val;
    break;
    // index control registers
  case ZYDIS_REGISTER_DI:
    RDI.w = val & 0xffff;
    break;
  case ZYDIS_REGISTER_EDI:
    RDI.d = val & 0xffffffff;
    break;
  case ZYDIS_REGISTER_RDI:
    RDI.q = val;
    break;
  case ZYDIS_REGISTER_SI:
    RSI.w = val & 0xffff;
    break;
  case ZYDIS_REGISTER_ESI:
    RSI.d = val & 0xffffffff;
    break;
  case ZYDIS_REGISTER_RSI:
    RSI.q = val;
    break;
    // other general purpose registers
  case ZYDIS_REGISTER_R8B:
    R8.l = val & 0xff;
    break;
  case ZYDIS_REGISTER_R8W:
    R8.w = val & 0xffff;
    break;
  case ZYDIS_REGISTER_R8D:
    R8.d = val & 0xffffffff;
    break;
  case ZYDIS_REGISTER_R8:
    R8.q = val;
    break;
  case ZYDIS_REGISTER_R9B:
    R9.l = val & 0xff;
    break;
  case ZYDIS_REGISTER_R9W:
    R9.w = val & 0xffff;
    break;
  case ZYDIS_REGISTER_R9D:
    R9.d = val & 0xffffffff;
    break;
  case ZYDIS_REGISTER_R9:
    R9.q = val;
    break;
  case ZYDIS_REGISTER_R10B:
    R10.l = val & 0xff;
    break;
  case ZYDIS_REGISTER_R10W:
    R10.w = val & 0xffff;
    break;
  case ZYDIS_REGISTER_R10D:
    R10.d = val & 0xffffffff;
    break;
  case ZYDIS_REGISTER_R10:
    R10.q = val;
    break;
  case ZYDIS_REGISTER_R11B:
    R11.l = val & 0xff;
    break;
  case ZYDIS_REGISTER_R11W:
    R11.w = val & 0xffff;
    break;
  case ZYDIS_REGISTER_R11D:
    R11.d = val & 0xffffffff;
    break;
  case ZYDIS_REGISTER_R11:
    R11.q = val;
    break;
  case ZYDIS_REGISTER_R12B:
    R12.l = val & 0xff;
    break;
  case ZYDIS_REGISTER_R12W:
    R12.w = val & 0xffff;
    break;
  case ZYDIS_REGISTER_R12D:
    R12.d = val & 0xffffffff;
    break;
  case ZYDIS_REGISTER_R12:
    R12.q = val;
    break;
  case ZYDIS_REGISTER_R13B:
    R13.l = val & 0xff;
    break;
  case ZYDIS_REGISTER_R13W:
    R13.w = val & 0xffff;
    break;
  case ZYDIS_REGISTER_R13D:
    R13.d = val & 0xffffffff;
    break;
  case ZYDIS_REGISTER_R13:
    R13.q = val;
    break;
  case ZYDIS_REGISTER_R14B:
    R14.l = val & 0xff;
    break;
  case ZYDIS_REGISTER_R14W:
    R14.w = val & 0xffff;
    break;
  case ZYDIS_REGISTER_R14D:
    R14.d = val & 0xffffffff;
    break;
  case ZYDIS_REGISTER_R14:
    R14.q = val;
    break;
  case ZYDIS_REGISTER_R15B:
    R15.l = val & 0xff;
    break;
  case ZYDIS_REGISTER_R15W:
    R15.w = val & 0xffff;
    break;
  case ZYDIS_REGISTER_R15D:
    R15.d = val & 0xffffffff;
    break;
  case ZYDIS_REGISTER_R15:
    R15.q = val;
    break;
  case ZYDIS_REGISTER_IP:
    RIP.w = val & 0xffff;
    break;
  case ZYDIS_REGISTER_EIP:
    RIP.d = val & 0xffffffff;
    break;
  case ZYDIS_REGISTER_RIP:
    RIP.q = val;
    break;
  case ZYDIS_REGISTER_FLAGS:
    FLAGS.w = val & 0xffff;
  case ZYDIS_REGISTER_EFLAGS:
    FLAGS.d = val & 0xffffffff;
  case ZYDIS_REGISTER_RFLAGS:
    FLAGS.q = val;
  case ZYDIS_REGISTER_ES:
  case ZYDIS_REGISTER_CS:
  case ZYDIS_REGISTER_SS:
  case ZYDIS_REGISTER_DS:
  case ZYDIS_REGISTER_FS:
  case ZYDIS_REGISTER_GS:
    throw std::exception("segment register not supported");
  default:
    throw std::exception("unrealized register");
  }
}

Registers::Registers() {}
Registers::~Registers() {}
} // namespace registers

} // namespace cpu
} // namespace backend
} // namespace core
