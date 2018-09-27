#include "backend/cpu/disasm.hpp"

namespace core {
namespace backend {
namespace cpu {
DisAsm::DisAsm(VirtualCpu &cpu_, VirtualMachine &vm_)
    : cpu(cpu_), formatter{}, decoder{}, vm(vm_) {
  // initialize the Zydis decoder
  ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64,
                   ZYDIS_ADDRESS_WIDTH_64);
  // and the formatter
  ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);
}

DisAsm::~DisAsm() {
}

InstructionInfo DisAsm::try_decode(uint64_t pointer, const void *data,
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
  std::cout << std::uppercase << std::hex << std::setw(sizeof uint64_t * 2) << std::setfill('0') << info.instruction_pointer << std::flush;
  char buffer[256];
  ZydisFormatterFormatInstruction(&formatter, &info.instruction, buffer,
                                  sizeof(buffer));

  std::cout << " " << buffer << std::endl;
}


bool DisAsm::try_execute(const InstructionInfo &info) {
  // try to execute the virtual machine
  return vm.execute_instruction(info.instruction);
}

void DisAsm::poll() {
  // get the instruction from cpu
  const auto inst = cpu.fetch();
  // instruction pointer
  auto inst_ptr = cpu.get_inst_pointer();

  // try to decode
  const auto inst_info = try_decode(inst_ptr, inst, 0xffff);

  // check for decoding error
  if (inst_info.instruction_pointer) {
    // set the instruction pointer to the next instruction address
    cpu.set_inst_pointer(inst_ptr + inst_info.instruction.length);
    // debug
    print(inst_info);

    if (!try_execute(inst_info)) {
      // wtf
      throw std::exception("failed to execute instruction");
    }
  } else {
    // todo: undefined opcode behaviour
    throw std::exception("unimplemented opcode");
  }
}

} // namespace cpu
} // namespace backend
} // namespace core


