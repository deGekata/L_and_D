#include "ElfGenerator.hpp"






void CreateElfHeader(FILE* output, uint64_t byteCodeSize, uint64_t dataSize) {
    fseek(output, 0, SEEK_SET);

    Elf64_Ehdr header = {};

    header.e_ident[0]   = 0x7f;
    header.e_ident[1]   = 0x45; // Взять из elf.h
    header.e_ident[2]   = 0x4c;
    header.e_ident[3]   = 0x46;
    header.e_ident[4]   = 0x02;
    header.e_ident[5]   = 0x01;
    header.e_ident[6]   = 0x01;

    header.e_type      = 0x02;
    header.e_machine   = 0x3e;
    header.e_version   = 0x01;
    header.e_entry     = 0x401000; // Взять константу ~НЕ из elf.h
    header.e_phoff     = 0x40;
    header.e_ehsize    = 0x40;
    header.e_phentsize = 0x38; 
    header.e_phnum     = 0x03;

    fwrite(&header, sizeof(header), 1, output);

    Elf64_Phdr firstProgramHeader = {};
    
    firstProgramHeader.p_type   = 0x01;
    firstProgramHeader.p_flags  = 0x04;
    firstProgramHeader.p_offset = 0x00;
    firstProgramHeader.p_vaddr  = 0x400000;
    firstProgramHeader.p_paddr  = 0x400000;
    firstProgramHeader.p_filesz = sizeof(Elf64_Ehdr) + 3 * sizeof(Elf64_Phdr);
    firstProgramHeader.p_memsz  = sizeof(Elf64_Ehdr) + 3 * sizeof(Elf64_Phdr);
    firstProgramHeader.p_align  = 0x1000;

    fwrite(&firstProgramHeader, sizeof(Elf64_Phdr), 1, output);

    Elf64_Phdr secondProgramHeader = {};
    
    secondProgramHeader.p_type   = 0x01;
    secondProgramHeader.p_flags  = 0x05; // Segment flags: read and execute
    secondProgramHeader.p_offset = 0x1000;
    secondProgramHeader.p_vaddr  = 0x401000;
    secondProgramHeader.p_paddr  = 0x401000;
    secondProgramHeader.p_filesz = byteCodeSize;
    secondProgramHeader.p_memsz  = byteCodeSize;
    secondProgramHeader.p_align  = 0x1000;

    fwrite(&secondProgramHeader, sizeof(Elf64_Phdr), 1, output);

    Elf64_Phdr thirdProgramHeader = {};
    
    thirdProgramHeader.p_type   = 0x01;
    thirdProgramHeader.p_flags  = 0x06; // Segment flags: read and execute
    thirdProgramHeader.p_offset = 0x2000;
    thirdProgramHeader.p_vaddr  = 0x402000;
    thirdProgramHeader.p_paddr  = 0x402000;
    thirdProgramHeader.p_filesz = dataSize;
    thirdProgramHeader.p_memsz  = dataSize;
    thirdProgramHeader.p_align  = 0x1000;

    fwrite(&thirdProgramHeader, sizeof(Elf64_Phdr), 1, output);
  
    fseek(output, ENTRY_POINT, SEEK_SET);
}


void CompileProgram(FILE* input, FILE* output) {
    
    FViewTable table;
    byte_code_instance* code = table.generate_code(input);

    CreateElfHeader(output, code->size, 0);
    
    fwrite(code->bytes, sizeof(unsigned char), code->size, output);

    return;
    
}
