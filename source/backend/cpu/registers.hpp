#ifndef REGISTERS_HPP_
#define REGISTERS_HPP_

#include <cstdint>
#include <exception>
#include <bitset>

#include <Zydis/Zydis.h>

namespace core {
namespace backend {
namespace cpu {
namespace registers {
static constexpr size_t ModRM_RegisterIndices_1632[8] = {0, 2, 3, 1,
                                                         7, 6, 4, 5};
static constexpr size_t ModRM_RegisterIndices_8[8] = {
    0, 2, 3, 1, 0, 2, 3, 1,
};

// enum x86_64Registers64 : uint8_t {
//  rax,
//  rbx,
//  rcx,
//  rdx,
//  rsi,
//  rdi,
//  rbp,
//  rsp,
//  r8,
//  r9,
//  r10,
//  r11,
//  r12,
//  r13,
//  r14,
//  r15
//};
//
// enum x86_64Registers32 : uint8_t {
//  eax = 16,
//  ebx,
//  ecx,
//  edx,
//  esi,
//  edi,
//  ebp,
//  esp,
//  r8d,
//  r9d,
//  r10d,
//  r11d,
//  r12d,
//  r13d,
//  r14d,
//  r15d
//};
//
// enum x86_64Registers16 : uint8_t {
//  ax = 32,
//  bx,
//  cx,
//  dx,
//  si,
//  di,
//  bp,
//  sp,
//  r8w,
//  r9w,
//  r10w,
//  r11w,
//  r12w,
//  r13w,
//  r14w,
//  r15w
//};
//
// enum x86_64RegistersLow : uint8_t {
//  al = 64,
//  bl,
//  cl,
//  dl,
//  sil,
//  dil,
//  bpl,
//  spl,
//  r8b,
//  r9b,
//  r10b,
//  r11b,
//  r12b,
//  r13b,
//  r14b,
//  r15b
//};
//
// enum x86RegistersHigh : uint8_t { ah = 128, bh, ch, dh, sih, dih, bph, sph };
//
// enum x86SegmentRegisters : uint8_t { cs, ds, es, fs, gs, ss };
//
// static constexpr size_t ZydisRegisterIndices[ZYDIS_REGISTER_MAX_VALUE] = {
//    0, al,   cl,   dl,   bl,   ah,   ch,   dh,   bh,   spl,  bpl,  sil,  dil,
//    r8b,  r9b,  r10b, r11b, r12b, r13b, r14b, r15b, ax,   cx,   dx,   bx,
//    sp,   bp,   si,   di,   r8w,  r9w,  r10w, r11w, r12w, r13w, r14w, r15w,
//    eax,  ecx,  edx,  ebx,  esp,  ebp,  esi,  edi,  r8d,  r9d,  r10d, r11d,
//    r12d, r13d, r14d, r15d, rax,  rcx,  rdx,  rbx,  rsp,  rbp,  rsi,  rdi,
//    r8,   r9,   r10,  r11,  r12,  r13,  r14,  r15
//};

enum FlagMasks : uint64_t {
  kCarryFlag = 0x0001,
  kParityFlag = 0x0004,
  kAdjustFlag = 0x0010,
  kZeroFlag = 0x0040,
  kSignFlag = 0x0080,
  kTrapFlag = 0x0100,
  kInterruptEnableFlag = 0x0200,
  kDirectionFlag = 0x0400,
  kOverflowFlag = 0x0800
};

class Registers {
public:
  union x86Reg {
  public:
    uint64_t q;
    uint32_t d;
    uint16_t w;
    uint8_t l;

    x86Reg() : q(0) {}
  };

  // General purpose registers 64-bit
  x86Reg RAX, RCX, RDX, RBX, RSP, RBP, RSI, RDI, R8, R9, R10, R11, R12, R13,
      R14, R15;
  // Flags register
  x86Reg FLAGS;
  // Instruction-pointer register
  x86Reg RIP;

  uint64_t read(ZydisRegister reg);
  void write(ZydisRegister reg, uint64_t value);

private:
public:
  Registers();
  ~Registers();
};

} // namespace registers
} // namespace cpu
} // namespace backend
} // namespace core

#endif // REGISTERS_HPP_
