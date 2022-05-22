#ifndef CODE_GENERATOR
#define CODE_GENERATOR

#include "Semantic_check.hpp"
#include <elf.h>

const uint32_t ELF_HEADER_SIZE     = 64;
const uint32_t PROGRAM_HEADER_SIZE = 56;
const uint32_t ENTRY_POINT         = 0x1000;
const uint32_t VIRTUAL_ENTRY_POINT = 0x401000;

void CreateElfHeader(FILE* output, uint64_t byteCodeSize, uint64_t dataSize);

void CompileProgram(FILE* input, FILE* output);


#endif