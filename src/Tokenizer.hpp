#ifndef TOKENIZER
#define TOKENIZER

#include "LangTree.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <cctype>

#define $deb printf("%d - cur_line\n", __LINE__);

struct Lexer {
    char* buffer;
    size_t cur_pos;
    size_t size;
};


void init_lexer(Lexer* lexer, FILE* file);

void destruct_lexer(Lexer* lexer);

Node* get_node(Lexer* lexer);

bool is_unnecessary(char chr);


int64_t get_file_size (FILE* inp);


#endif