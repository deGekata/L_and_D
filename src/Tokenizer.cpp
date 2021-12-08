#include "Tokenizer.hpp"


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
    $deb

    if (lexer->cur_token) return lexer->cur_token;
    $deb

    skip_unnecessary(lexer);
    $deb

    if (lexer->cur_pos >= lexer->size) {
        return NULL;
    }
    $deb
    
    lexer->cur_token = NULL;
    $deb
    if ('0' <= lexer->buffer[lexer->cur_pos] && lexer->buffer[lexer->cur_pos] <= '9') {
        lexer->cur_token = try_get_num(lexer);
        if (lexer->cur_token != NULL) return lexer->cur_token;
    }
    $deb

    lexer->cur_token =  try_get_operator(lexer);
    if (lexer->cur_token != NULL) {
        return lexer->cur_token;
    }
    $deb
    Node* node = (Node*) calloc(1, sizeof(Node));
    lexer->cur_token = try_get_name(lexer);
    if (lexer->cur_token != NULL) {
        return lexer->cur_token;
    }
    $deb
    
    lexer->cur_token = try_get_special(lexer);
    if (lexer->cur_token != NULL) {
        return lexer->cur_token;
    }
    $deb

    return lexer->cur_token;
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
    }

    ret_node->type = NodeType::NUMBER;
    ret_node->data.num = ret_num;

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
            ret_node->data.opr =  Operator::NOT;
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

    Node* ret_node = (Node*) calloc(1, sizeof(Node*));
    ret_node->type = NodeType::IDENTIFIER;
    ret_node->pos  = lexer->line_pos;
    
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

    return ret_node;


}

Node* try_get_special(Lexer* lexer) {
    assert(lexer && "must not be null");

    if (lexer->cur_pos >= lexer->size) return NULL;

    Node* ret_node = (Node*) calloc(1, sizeof(Node));



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
        break;

    default:
        break;
    }
    return ret_node;
}

bool is_unnecessary(char chr) {
    return isspace(chr) || chr == '\n';
}

void skip_unnecessary(Lexer* lexer) {
    assert(lexer && "must not be null");
    assert(lexer->buffer && "must not be null");
    
    while (lexer->cur_pos < lexer->size && is_unnecessary(lexer->buffer[lexer->cur_pos])) {
        lexer->line_pos = lexer->buffer[lexer->cur_pos] == '\n' ? 0 : lexer->line_pos + 1;
        lexer->line += lexer->buffer[lexer->cur_pos] == '\n';
        lexer->cur_pos++;
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

int64_t hashFunc_(const char * str, size_t len, int64_t init) {
    unsigned long long int hash = init;
    for (size_t it = 0; it < len; str++, it++) {
        hash += (unsigned char)(*str);
        hash += (hash << 20);
        hash ^= (hash >> 12);
    }

    hash += (hash << 6);
    hash ^= (hash >> 22);
    hash += (hash << 29);

    return hash;
}