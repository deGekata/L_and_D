#include "Tokenizer.hpp"


void init_lexer (Lexer* lexer, FILE* file) {
    $deb
    assert(lexer && "must not be null");
    $deb
    assert(file && "must not be null");
    $deb
    int64_t file_size = get_file_size(file);
    $deb
    char* buff = (char*) calloc(file_size+20, sizeof(char));
    $deb
    fread (buff, sizeof(char), file_size, file);
    $deb
    printf("'%s' in file", buff);

    for (size_t slow_ptr = 0,  fast_ptr = 0; fast_ptr < file_size; ++fast_ptr) {
        if (!is_unnecessary(buff[fast_ptr])) {
            buff
        }
    }


}

bool is_unnecessary(char chr) {
    return isspace(chr) || chr == '\n';
}


int64_t get_file_size (FILE* inp) {
    assert(inp);
    fseek(inp, 0L, SEEK_END);
    int64_t size = ftell(inp);
    fseek(inp, 0L, SEEK_SET);
    return size;
}