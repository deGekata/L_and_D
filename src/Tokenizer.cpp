#include "Tokenizer.hpp"



#define BASE_OPERATOR_HASHES_CAP 30
reserved_item* operator_hashes = 0;
size_t operator_hashes_size = 0;
size_t operator_hashes_cap  = 0;

#define new_command(keyword, res_op) \
    if (operator_hashes_size == operator_hashes_cap) {\
        operator_hashes = (reserved_item*) realloc(operator_hashes, sizeof(*operator_hashes) * operator_hashes_cap * 2);\
        assert(operator_hashes && "to many opeartor hashes, no enough memory");\
        operator_hashes_cap *= 2;\
    }\
    operator_hashes[operator_hashes_size].hash = hash_func(keyword, strlen(keyword), 0); \
    operator_hashes[operator_hashes_size++].op = res_op;
    


void init_operator_hashes() {
    if (operator_hashes == 0) {
        operator_hashes = (reserved_item*) calloc(BASE_OPERATOR_HASHES_CAP, sizeof(reserved_item));
        operator_hashes_cap = BASE_OPERATOR_HASHES_CAP;
        operator_hashes_size = 0;
    }
    new_command("while", Operator::WHILE);
    printf("while hash = %d\n", operator_hashes[operator_hashes_size - 1]);
    new_command("var", Operator::VAR);
    printf("var hash = %zu\n", operator_hashes[operator_hashes_size - 1]);
    new_command("func", Operator::FUNC);
    printf("func hash = %zu\n", operator_hashes[operator_hashes_size - 1]);
    new_command("if", Operator::TERN_Q);
    printf("func hash = %zu\n", operator_hashes[operator_hashes_size - 1]);
    new_command("else", Operator::TERN_C);
    printf("func hash = %zu\n", operator_hashes[operator_hashes_size - 1]);

}

void init_lexer (Lexer* lexer, FILE* file) {
    assert(lexer && "must not be null");
    assert(file && "must not be null");

    size_t file_size = get_file_size(file);
    char* buff = (char*) calloc(file_size, sizeof(char));
    fread (buff, sizeof(char), file_size, file);

    lexer->buffer = buff;
    lexer->size = file_size; //slow_ptr;
    lexer->cur_pos = 0;

}

void pop_node(Lexer* lexer) {
    assert(lexer && "must not be null");
    assert(lexer->cur_token);

    lexer->cur_token = NULL;
}

Node* get_node(Lexer* lexer) {
    assert(lexer && "must not be null");
    // $deb

    if (lexer->cur_token) return lexer->cur_token;
    // $deb

    skip_unnecessary(lexer);
    // $deb

    if (lexer->cur_pos >= lexer->size) {
        return NULL;
    }
    // $deb
    
    lexer->cur_token = NULL;
    // $deb
    if ('0' <= lexer->buffer[lexer->cur_pos] && lexer->buffer[lexer->cur_pos] <= '9') {
        lexer->cur_token = try_get_num(lexer);
        if (lexer->cur_token != NULL) return lexer->cur_token;
    }
    // $deb

    lexer->cur_token =  try_get_operator(lexer);
    if (lexer->cur_token != NULL) {
        return lexer->cur_token;
    }
    // $deb
    // Node* node = (Node*) calloc(1, sizeof(Node));
    lexer->cur_token = try_get_name(lexer);
    if (lexer->cur_token != NULL) {
        return lexer->cur_token;
    }
    // $deb
    
    lexer->cur_token = try_get_special(lexer);
    if (lexer->cur_token != NULL) {
        return lexer->cur_token;
    }
    // $deb

    return NULL;
    // return lexer->cur_token;
}

Node* try_get_num(Lexer* lexer) {
    assert(lexer && "must not be null");
    
    if (lexer->cur_pos >= lexer->size || !isdigit(lexer->buffer[lexer->cur_pos])) {
        printf("wtf\n");
        return NULL;
    }
    
    Node* ret_node = (Node*) calloc(1, sizeof(Node));
    ret_node->line = lexer->line;
    ret_node->pos  = lexer->line_pos;
    int ret_num = 0;

    while (lexer->cur_pos < lexer->size && isdigit(lexer->buffer[lexer->cur_pos])) {
        ret_num = ret_num * 10 + (lexer->buffer[lexer->cur_pos++] - '0');
        lexer->line_pos++;
        printf("%d line pos\n", lexer->line_pos);
    }

    ret_node->type = NodeType::NUMBER;
    ret_node->data.num = ret_num    ;
    ret_node->line = lexer->line;
    ret_node->pos = lexer->line_pos;
    return ret_node;
}

Node* try_get_operator(Lexer* lexer) {
    assert(lexer && "must not be null");

    if (lexer->cur_pos >= lexer->size) return NULL;

    Node* ret_node = (Node*) calloc(1, sizeof(Node));

    if (lexer->cur_pos < lexer->size - 1) 
        switch (lexer->buffer[lexer->cur_pos]*256 + lexer->buffer[lexer->cur_pos + 1]) {
        case '++':
            ret_node->data.opr = Operator::INC;
            break;
        
        case '--':
            ret_node->data.opr = Operator::DEC;
            break;
        
        case '>=':
            ret_node->data.opr = Operator::GRTR_EQ;
            break;
        
        case '<=':
            ret_node->data.opr = Operator::LESS_EQ;
            break;
        
        case '==':
            ret_node->data.opr = Operator::EQUAL;
            break;
        
        case '!=':
            ret_node->data.opr = Operator::NON_EQ;
            break;
        
        case '<<':
            ret_node->data.opr = Operator::SHL;
            break;
        
        case '>>':
            ret_node->data.opr = Operator::SHR;
            break;
        
        case '||':
            ret_node->data.opr = Operator::OR;
            break;
        
        case '&&':
            ret_node->data.opr = Operator::AND;
            break;
        
        case '??':
            ret_node->data.opr = Operator::QQ;
            break;
        
        default:
            ret_node->data.opr = Operator::NONE;
            break;
        }

    if (ret_node->data.opr != Operator::NONE) {
        ret_node->type  =  NodeType::OPERATOR;
        ret_node->line  =  lexer->line;
        ret_node->pos   =  lexer->line_pos;
        lexer->line_pos += 2;
        lexer->cur_pos  += 2;
        return ret_node;
    }

    switch (lexer->buffer[lexer->cur_pos]) {
        case '+':
            ret_node->data.opr =  Operator::ADD;
            break;
        
        case '-':
            ret_node->data.opr =  Operator::SUB;
            break;
        
        case '*':
            ret_node->data.opr =  Operator::MUL;
            break;
        
        case '/':
            ret_node->data.opr =  Operator::DIV;
            break;
        
        case '%':
            ret_node->data.opr =  Operator::MOD;
            break;
        
        case '>':
            ret_node->data.opr =  Operator::GRTR;
            break;
        
        case '<':
            ret_node->data.opr =  Operator::LESS;
            break;
        
        case '=':
            ret_node->data.opr =  Operator::SET;
            break;
        
        case '!':
            ret_node->data.opr =  Operator::NOT;
            break;
        
        case '^':
            ret_node->data.opr =  Operator::XOR;
            break;
        
        case '@':
            ret_node->data.opr =  Operator::ADDR;
            break;
        
        
        // case '|':
        //     return Operator::;
        //     break;
        
        // case '&':
        //     return Operator::AND;
        //     break;
        
        default:
            ret_node->data.opr = Operator::NONE;
            break;
    }
    
    if (ret_node->data.opr != Operator::NONE) {
        ret_node->type  =  NodeType::OPERATOR;
        ret_node->line  =  lexer->line;
        ret_node->pos   =  lexer->cur_pos;
        lexer->line_pos += 1;
        lexer->cur_pos  += 1;
        return ret_node;
    }

    free(ret_node);

    return NULL;
}

Node* try_get_name(Lexer* lexer) {
    assert(lexer && "must not be null");

    if (lexer->cur_pos >= lexer->size) return NULL;

    Node* ret_node = (Node*) calloc(1, sizeof(Node));
    ret_node->type = NodeType::IDENTIFIER;
    ret_node->line = lexer->line;
    ret_node->pos = lexer->line_pos;
    
    size_t len = 0;
    while (isalpha(lexer->buffer[lexer->cur_pos]) || lexer->buffer[lexer->cur_pos] == '_') {
        lexer->cur_pos++, lexer->line_pos++;
        len++;
    }

    if (len == 0) {
        free(ret_node);
        return NULL;
    }

    #ifdef NODE_DEBUG_INFO
        ret_node->name = (char*) calloc(len + 1, sizeof(char));
        memcpy(ret_node->name, lexer->buffer + lexer->cur_pos - len, len);
        ret_node->name[len] = '\0';
    #endif

    ret_node->data.id = (idt_t) hash_func(ret_node->name, len, 0);
    try_name_to_operator(ret_node);
    printf("ret_node id = %zu, '%s'\n", ret_node->data.id, ret_node->name);



    return ret_node;
}

Node* try_get_special(Lexer* lexer) {
    assert(lexer && "must not be null");

    if (lexer->cur_pos >= lexer->size) return NULL;

    Node* ret_node = (Node*) calloc(1, sizeof(Node));
    ret_node->type = NodeType::NONE;


    switch (lexer->buffer[lexer->cur_pos]) {
    case ';':
        ret_node->type = NodeType::OPERATOR;
        ret_node->data.opr = Operator::ENDL;
        break;

    case ',':
        ret_node->type = NodeType::OPERATOR;
        ret_node->data.opr = Operator::COMMA;
        break;

    case '(':
        ret_node->type = NodeType::CUSTOM;
        ret_node->data.custom = '(';
        break;

    case ')':
        ret_node->type = NodeType::CUSTOM;
        ret_node->data.custom = ')';
        printf("closing\n");
        break;

    case '{':
        ret_node->type = NodeType::CUSTOM;
        ret_node->data.custom = '{';
        printf("opening wavy\n");
        break;

    case '}':
        ret_node->type = NodeType::CUSTOM;
        ret_node->data.custom = '}';
        printf("closing wavy\n");
        break;

    

    default:
        break;
    }

    if (ret_node->type != NodeType::NONE) {
        ret_node->line  =  lexer->line;
        ret_node->pos   =  lexer->cur_pos;
        lexer->line_pos += 1;
        lexer->cur_pos  += 1;
        return ret_node;
    }
    free(ret_node);
    return NULL;
}

void try_name_to_operator(Node* node) {
    assert(node && "node must not be NULL");
    assert(operator_hashes && "operator_hashes must not be NULL");

    for (size_t it = 0; it < operator_hashes_size; ++it) {
        if (node->data.id == operator_hashes[it].hash) {
            node->type = NodeType::OPERATOR;
            node->data.opr = operator_hashes[it].op;
            printf("found some stuff %d\n", node->data.opr);
            return;
        }
        
    }
    return;
}

bool is_unnecessary(char chr) {
    return isspace(chr) || chr == '\n';
}

void skip_unnecessary(Lexer* lexer) {
    assert(lexer && "must not be null");
    assert(lexer->buffer && "must not be null");
    
    while (lexer->cur_pos < lexer->size && is_unnecessary(lexer->buffer[lexer->cur_pos])) {
        lexer->line_pos = (lexer->buffer[lexer->cur_pos] == '\n') ? 0 : lexer->line_pos + 1;
        lexer->line += lexer->buffer[lexer->cur_pos] == '\n';
        if (lexer->buffer[lexer->cur_pos] == '\n') printf("----------------------------new line\n");
        lexer->cur_pos++;   
        // printf("%d line pos _s\n", lexer->line_pos);
    }

    return;
}

size_t get_file_size (FILE* inp) {
    assert(inp);
    fseek(inp, 0L, SEEK_END);
    size_t size = size_t(ftell(inp));
    fseek(inp, 0L, SEEK_SET);
    return size;
}

int64_t hash_func(const char * str, size_t len, int64_t init) {
    unsigned long long int hash = init;
    printf("str = '%s', strlen = %d \n", str, len);
    for (size_t it = 0; it < len; str++, it++) {
        printf("it = %d, sym:'%c', hash = %zu\n", it, *str, hash);
        hash += (unsigned char)(*str);
        hash += (hash << 20);
        hash ^= (hash >> 12);
    }

    hash += (hash << 6);
    hash ^= (hash >> 22);
    hash += (hash << 29);

    return hash;
}