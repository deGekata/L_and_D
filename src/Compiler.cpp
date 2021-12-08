#include "Compiler.hpp"


bool compile_programm(FILE* input, FILE* output) {
    assert(input && "input ptr must not be null");
    assert(output && "input ptr must not be null");

    Lexer* lexer = (Lexer*) calloc(1, sizeof(Lexer));
    init_lexer(lexer, input);

    


    return true;
}