#include "backend/cpu/disasm.hpp"

namespace core {
namespace backend {
namespace cpu {
DisAsm::DisAsm(VirtualCpu& cpu_) : cpu(cpu_), formatter{}, decoder{} {
  // initialize the Zydis decoder
  ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64,
                   ZYDIS_ADDRESS_WIDTH_64);
  // and the formatter
  ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);
}

DisAsm::~DisAsm() {
}

const InstructionInfo &DisAsm::try_decode(uint64_t pointer, const void *data,
                                          size_t length) {
  ZydisDecodedInstruction instruction;
  // try to decode the instruction 
  if (ZYDIS_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, data, length, pointer,
                                             &instruction))) {
    // decode succed, return the result
    return {pointer, instruction};
  } else {
    // return empty struct
    // caller must check if pointer != null for success
    return {};
  }
}
void DisAsm::print(const InstructionInfo &info) {
  std::cout << std::hex << std::setw(sizeof uint64_t * 2) << std::setfill('0') << info.instruction_pointer << std::flush;
  char buffer[256];
  ZydisFormatterFormatInstruction(&formatter, &info.instruction, buffer,
                                  sizeof(buffer));

  std::cout << " " << buffer << std::endl;
}


bool DisAsm::try_execute(const InstructionInfo &info) {


  return false;
}
} // namespace cpu
} // namespace backend
} // namespace core


