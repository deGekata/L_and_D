#include "LangTree.hpp"
#include <stdio.h>
#include "Tokenizer.hpp"

int main() {
    Lexer* lexer = (Lexer*) calloc(1, sizeof(Lexer));
    FILE* file = fopen("test.txt", "r");

    init_lexer(lexer, file);

    return 0;
}