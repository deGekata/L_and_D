#ifndef TOKENIZER
#define TOKENIZER

#include "LangTree.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <cctype>
#include <string.h>
#include <inttypes.h>

#define $deb printf("%d - cur_line\n", __LINE__);


struct reserved_item {
    idt_t hash;
    Operator op;
};

struct Lexer {
    size_t line = 0;
    size_t line_pos = 0;

    char* buffer = NULL;
    size_t cur_pos = 0;
    size_t size = 0;

    Node* cur_token = NULL;
};

void init_operator_hashes();

void init_lexer(Lexer* lexer, FILE* file);

void destruct_lexer(Lexer* lexer);

void pop_node(Lexer* lexer);


Node* get_node(Lexer* lexer);

Node* try_get_num(Lexer* lexer);

Node* try_get_operator(Lexer* lexer);

Node* try_get_name(Lexer* lexer);

Node* try_get_special(Lexer* lexer);

void try_name_to_operator(Node* node);

bool is_operator(char chr);

bool is_unnecessary(char chr);
    
void skip_unnecessary(Lexer* lexer);

size_t get_file_size (FILE* inp);

int64_t hash_func(const char * str, size_t len, int64_t init);


#endif