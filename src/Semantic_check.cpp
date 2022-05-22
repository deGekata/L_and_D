#include "Semantic_check.hpp"

#define GENERATE_BINARY_STANDART(OPER_NAME, lower_case) \
    case Operator:: OPER_NAME : {\
        generate_all(root->left);\
        generate_all(root->right);\
        byte_code_instance* instance = get_ ## lower_case ## _seq_bytes();\
        this->buff->addCodeToBuff(NULL, instance->bytes, instance->size);\
        break;\
    };
            

ProgramBuff::~ProgramBuff() {

    // for (size_t it = 0; it < this->program.size(); ++it)
        // free(program[it]);
    // progam.~MyVector();
    // binary_program.~MyVector();
}


//TODO: add bytes to binary program
void ProgramBuff::addCodeToBuff(char* listing, unsigned char* bytes, size_t bytes_len) {
    // if (listing == NULL) {
    //     return;
    // }
    // this->program.push_back(listing);
    for (size_t it = 0; it < bytes_len; ++it) {
        this->binary_program.push_back(bytes[it]);
    }
    return;
}

void ProgramBuff::writeLittleEndianInt(size_t offset, int value) {
    for (size_t cur_pos = offset; cur_pos < (offset + 4); ++cur_pos) {
        this->binary_program[cur_pos] = (char)(value & 0xff);
        value = value >> 8;
    }
    return;
}


size_t ProgramBuff::bin_program_size() const {
    return this->binary_program.size();
}

void ProgramBuff::dropBuffToFile(FILE* file) {
    assert(file && "file ptr must not be NULL");
    
    fwrite(this->binary_program.data(), sizeof(unsigned char), binary_program.size(), file);
    return;
}

void ProgramBuff::dropBuffToArray(unsigned char* arr) {
    assert(arr);

    memcpy(arr, this->binary_program.data(), binary_program.size());
    return;
}


int FViewTable::insertEntry(size_t id, char* name) {
    for (size_t it = 0; it < members.size(); ++it) {
        if (strcmp(members[it].name, name) == 0) {
            return 1;
        }
    }

    members.push_back( Entry(id, name, 0));
    return 0;
} //return 1 if error
    
int FViewTable::insertEntry(char* name) {
    for (size_t it = 0; it < members.size(); ++it) {
        if (strcmp(members[it].name, name) == 0) {
            return 1;
        }
    }

    members.push_back(Entry(0, name, 0));
    return 0;
} //return 1 if error

int FViewTable::insertEntry(char* name, int64_t offset) {
    for (size_t it = 0; it < members.size(); ++it) {
        if (strcmp(members[it].name, name) == 0) {
            return 1;
        }
    }
    // FViewTable* temp_table = new FViewTable;
    // func_tables.push_back(temp_table);
    Entry* temp = new Entry(0, name, offset + table_offset);
    members.push_back(*temp);
    return 0;
} //return 1 if error

int FViewTable::insertEntry(size_t id, char* name, int64_t offset) {
    for (size_t it = 0; it < members.size(); ++it) {
        if (strcmp(members[it].name, name) == 0) {
            return 1;
        }
    }
    Entry* temp_table = new Entry(id, name, offset);
    members.push_back(*temp_table);
    return 0;
} //return 1 if error


Entry* FViewTable::findEntry(char* name) {
    FViewTable* cur_table = this;
    while (cur_table != NULL) {
        for (size_t it = 0; it < cur_table->members.size(); ++it) {
            if (strcmp(name, cur_table->members[it].name) == 0) {
                return &(cur_table->members[it]);
            }
        }
        cur_table = cur_table->parent;
    }
    return NULL;
}

void FViewTable::update_size() {
    int max_child_sz = 0;
    for (size_t it = 0; it < childs.size(); ++it) {
        max_child_sz = (childs[it].current_size + childs[it].max_child_size) > max_child_sz ? (childs[it].current_size + childs[it].max_child_size) : max_child_sz;
    }
    if (max_child_sz > this->max_child_size) {
        // current_size = current_size - this->max_child_size + max_child_sz;
        this->max_child_size = max_child_sz;
    }
    return;
}


FViewTable& FViewTable::operator=(const FViewTable& rhs) {
    // this->buff = rhs.buff;
    // this->parent = parent;
    // this->childs = rhs.childs;
    // this->members = rhs.members;
    // this->unresolved_calls =unresolved_calls;

    this->current_size = rhs.current_size;
    this->table_offset = rhs.table_offset;
    this->max_child_size = rhs.max_child_size;
    this->buff = rhs.buff;
    this->parent = rhs.parent;
    this->childs = rhs.childs;
    this->members = rhs.members;
    this->unresolved_calls = rhs.unresolved_calls;
    this->unresolved_returns = rhs.unresolved_returns;
    return *this;
}



byte_code_instance* FViewTable::generate_code(FILE* file) {
    
    Lexer* lexer = (Lexer*) calloc(1, sizeof(Lexer));
    init_lexer(lexer, file);
    init_operator_hashes();
    
    Vector<Node*> func_trees = parse_functions(lexer);
    FViewTable func_name_table = FViewTable();

    // Node* node = parse_func_decl(lexer);
    // if (node == NULL && get_node(lexer) != NULL) assert(0 && "error while parsing tree"); 
    // printf("node is %d\n", node);
    Vector<FViewTable> func_tables;

    for (size_t it = 0; it < func_trees.size(); ++it) {
        bool is_not_ok = func_name_table.insertEntry(func_trees[it]->left->name);
        FViewTable temp_table = FViewTable();
        func_tables.push_back(temp_table);
        if (is_not_ok) {
            fprintf(stderr, "found duplication in func_trees name");
            assert(0);
        }
        draw_tree(func_trees[it]);
    }

    for (size_t it = 0; it < func_tables.size(); ++it) {
        func_tables[it].generate_func_params(func_trees[it]);
        func_tables[it].check_func_params(func_name_table);
        func_tables[it].buff = new ProgramBuff();
    }

    for (size_t it = 0; it < func_tables.size(); ++it) {
        func_tables[it].generate_func_body(func_trees[it]);
    }

    for (size_t it = 1; it < func_tables.size(); ++it) {
        func_tables[it].global_code_offset = func_tables[it - 1].buff->bin_program_size() + func_tables[it - 1].global_code_offset;
        func_name_table.members[it].offset = func_tables[it].global_code_offset;
        it + it;
    }

    for (size_t it = 0; it < func_tables.size(); ++it) {
        func_tables[it].process_calls(func_tables[it].global_code_offset, func_name_table);
    }

    

    
    uint8_t HLT_FUNC[] = {
        0xE8, 0x00, 0x00, 0x00, 0x00,   // call main (need to find main)
        0xb8, 0x3c, 0x00, 0x00, 0x00,   // mov eax, 0x3c
        0x48, 0x31, 0xff,               // xor rdi, rdi
        0x0f, 0x05};                    // syscall
    
    const uint32_t HLT_SIZE  = sizeof(HLT_FUNC);

    uint64_t code_size = HLT_SIZE;
    int32_t main_pos = 5;
    for (size_t it = 0; it < func_tables.size(); ++it) {
        code_size += func_tables[it].buff->bin_program_size();
        if (strcmp(func_name_table.members[it].name, "main") == 0) {
            main_pos = func_name_table.members[it].offset + HLT_SIZE - 5;
        }
    }

    for (size_t it = 1; it < 5; ++it) {
        HLT_FUNC[it] = (char)(main_pos & 0xff);
        main_pos = main_pos >> 8;
    }

    


    unsigned char* bytes = (unsigned char*) calloc(code_size + 1, sizeof(unsigned char));

    byte_code_instance* instance = (byte_code_instance*) calloc(1, sizeof(byte_code_instance));


    instance->bytes = bytes;
    instance->size = code_size;

    for (size_t it = 0; it < HLT_SIZE; ++it) {
        bytes[it] = HLT_FUNC[it];
    }   
    bytes += HLT_SIZE;

    for (size_t it = 0; it < func_tables.size(); ++it) {
        func_tables[it].buff->dropBuffToArray(bytes);
        bytes += func_tables[it].buff->bin_program_size();
    }
    
        
    
    return instance;

}

void FViewTable::generate_func_params(Node* node) {
    Node* cur_node = node->right->left;
    if (cur_node == NULL) return;

    int64_t max_offset = -24;  //return seg + return offset + pushed rbp

    if (cur_node->type == NodeType::OPERATOR) {
        while (cur_node != NULL && cur_node->type == NodeType::OPERATOR && cur_node->data.opr == Operator::COMMA) {
            max_offset -= 8;
            cur_node = cur_node->right;
            printf("looool\n");
        }

        cur_node = node->right->left;

        while (cur_node->type == NodeType::OPERATOR && cur_node->data.opr == Operator::COMMA) {
            bool is_not_ok = this->insertEntry(cur_node->left->left->name, max_offset);
            if (is_not_ok) {
                fprintf(stderr, "found duplication in variables");
            }
            max_offset += 8;
            cur_node = cur_node->right;
        }
    }

    bool is_not_ok = this->insertEntry(cur_node->left->name, max_offset);
    if (is_not_ok) {
        fprintf(stderr, "found duplication in variables");
    }
    // table.insertEntry(cur_node->name, max_offset);
    return;
}

void FViewTable::check_func_params(FViewTable& functions_table) {
    for (size_t it_var = 0; it_var < this->members.size(); ++it_var) {
        for (size_t it_func = 0; it_func < functions_table.members.size(); ++it_func) {
            if (strcmp(functions_table.members[it_func].name, this->members[it_var].name) == 0) {
                fprintf(stderr, "found collision of function param and function name: function '%s' and variable '%s'\n", functions_table.members[it_func].name, this->members[it_var].name);
                assert(0);
            }
        }
    }
}


void FViewTable::generate_func_body(Node* func_root) {
    Node* code_root = func_root->right->right->left;
    byte_code_instance* begin_block_inst = get_func_body_begin_seq_bytes();
    this->buff->addCodeToBuff(NULL, begin_block_inst->bytes, begin_block_inst->size);
    
    int rsp_diff_pos = buff->bin_program_size() - IF_JMP_COND_MAGIC_OFFSET;
    generate_all(code_root);

    int end_block_ret_offset = buff->bin_program_size();
    byte_code_instance* end_block_inst = get_func_body_end_seq_bytes();
    this->buff->addCodeToBuff(NULL, end_block_inst->bytes, end_block_inst->size);
    int rsp_diff_sub_pos = this->buff->bin_program_size() - 6;

    int max_block_size = this->current_size + this->max_child_size;

    this->buff->writeLittleEndianInt(rsp_diff_pos, max_block_size);
    this->buff->writeLittleEndianInt(rsp_diff_sub_pos, max_block_size);

    process_return_jmps(end_block_ret_offset);
    return;
}



void FViewTable::generate_all(Node* root) {
    
    switch (root->type) {
        case NodeType::NUMBER: {
            byte_code_instance* instance = get_imm_const_seq_bytes(root->data.num);
            this->buff->addCodeToBuff(NULL, instance->bytes, instance->size);
            break;
        };

        case NodeType::IDENTIFIER: {
            Entry* variable = findEntry(root->name);
            if (variable == NULL) {
                fprintf(stderr, "this '%s' variable hasn`t been found\n", root->name);
                assert(0);
            }
            byte_code_instance* instance = get_local_var_seq_bytes(variable->offset);
            this->buff->addCodeToBuff(NULL, instance->bytes, instance->size);
            break;
        };
        
        case NodeType::OPERATOR: {
            switch (root->data.opr) {
                case Operator::SET: {
                    if (root->left == NULL || root->left->type != NodeType::IDENTIFIER) {
                        fprintf(stderr, "chel, prisvaivay v variable...");
                        assert(0);
                    }
                    // generate_all(root->left);
                    Entry* var_entr = findEntry(root->left->name);
                    if (var_entr == NULL) {
                        fprintf(stderr, "chel, takoi variable net...");
                        assert(0);
                    }
                    generate_all(root->right);
                    byte_code_instance* instance = get_set_seq_bytes(var_entr->offset);
                    this->buff->addCodeToBuff(NULL, instance->bytes, instance->size); 
                    break;
                }

                case Operator::VAR: {
                    bool is_not_ok = this->insertEntry(root->left->name, this->current_size + 8);
                    if (is_not_ok) {
                        fprintf(stderr, "found duplication in variables");
                    }
                    this->current_size += 8;
                    if (root->next != NULL) {
                        generate_all(root->next);
                    }
                    break;
                }

                case Operator::ENDL: {
                    generate_all(root->left);
                    byte_code_instance* instance = get_endl_seq_bytes();
                    this->buff->addCodeToBuff(NULL, instance->bytes, instance->size);
                    if (root->next != NULL) {
                        generate_all(root->next);
                    }
                    break;
                }

                case Operator::F_VIEW: {
                    FViewTable* n_table = new FViewTable(this, this->buff, this->current_size);
                    this->childs.push_back(*n_table);
                    this->childs[this->childs.size() - 1].generate_all(root->left);
                    this->update_size();
                    // generate_all(root->left);
                    // byte_code_instance* instance = get_endl_seq_bytes();
                    // this->buff->addCodeToBuff(NULL, instance->bytes, instance->size);
                    if (root->next != NULL) {
                        generate_all(root->next);
                    }
                    break;
                }

                case Operator::TERN_Q: {
                    generate_all(root->left);

                    byte_code_instance* instance = get_if_cond_seq_bytes();
                    this->buff->addCodeToBuff(NULL, instance->bytes, instance->size);
                    int if_cond_jmp_offset = buff->bin_program_size() - IF_JMP_COND_MAGIC_OFFSET;

                    if (root->right->type == NodeType::OPERATOR && root->right->data.opr == Operator::TERN_C) {
                        generate_all(root->right->left);

                        byte_code_instance* else_inst = get_else_while_seq_bytes();
                        this->buff->addCodeToBuff(NULL, else_inst->bytes, else_inst->size);
                        int end_block_jmp = buff->bin_program_size() - IF_JMP_COND_MAGIC_OFFSET;

                        int mid = buff->bin_program_size() - if_cond_jmp_offset - IF_JMP_COND_MAGIC_OFFSET;
                        buff->writeLittleEndianInt(if_cond_jmp_offset, mid); //write offset for conditional jmp to else block

                        generate_all(root->right->right);
                        
                        mid = buff->bin_program_size() - end_block_jmp - IF_JMP_COND_MAGIC_OFFSET;
                        buff->writeLittleEndianInt(end_block_jmp, mid); //write offset for constant jmp to end of if block
                    } else {
                        generate_all(root->right);
                        int mid = buff->bin_program_size() - if_cond_jmp_offset - IF_JMP_COND_MAGIC_OFFSET;
                        buff->writeLittleEndianInt(if_cond_jmp_offset,  mid);
                    }

                    if (root->next != NULL) {
                        generate_all(root->next);
                    }
                    break;
                }

                case Operator::WHILE: {
                    int while_cond_jmp_offset = buff->bin_program_size();
                    generate_all(root->left);

                    byte_code_instance* instance = get_while_cond_seq_bytes();
                    this->buff->addCodeToBuff(NULL, instance->bytes, instance->size);

                    int cond_false_jmp_offset = buff->bin_program_size() - IF_JMP_COND_MAGIC_OFFSET;

                    generate_all(root->right);

                    byte_code_instance* end_jump_instance = get_else_while_seq_bytes();
                    this->buff->addCodeToBuff(NULL, end_jump_instance->bytes, end_jump_instance->size);
        
                    // int mid = buff->bin_program_size() - while_cond_jmp_offset - IF_JMP_COND_MAGIC_OFFSET;
                    int mid = while_cond_jmp_offset - (int)buff->bin_program_size();
                    buff->writeLittleEndianInt(cond_false_jmp_offset, (int)buff->bin_program_size() - cond_false_jmp_offset - IF_JMP_COND_MAGIC_OFFSET);
                    buff->writeLittleEndianInt(buff->bin_program_size() - IF_JMP_COND_MAGIC_OFFSET, while_cond_jmp_offset - (int)buff->bin_program_size());


                    if (root->next != NULL) {
                        generate_all(root->next);
                    }
                    break;
                }

                case Operator::CALL: {
                    Node* params = root->right;

                    int pop_cnt = 0;
                    if (params != NULL) {
                        while (params->type == NodeType::OPERATOR) {
                            generate_all(params->left);
                            pop_cnt++;
                            params = params->right;
                        }
                        generate_all(params);
                        pop_cnt++;
                    }
                    
                    
                    byte_code_instance* instance = get_call_seq_bytes();
                    buff->addCodeToBuff(NULL, instance->bytes, instance->size);
                    int call_pos = buff->bin_program_size() - IF_JMP_COND_MAGIC_OFFSET;
                    
                    for (int it = 0; it < pop_cnt; ++it) {
                        byte_code_instance* instance_pop = get_endl_seq_bytes();
                        buff->addCodeToBuff(NULL, instance_pop->bytes, instance_pop->size);
                    }
                    
                    byte_code_instance* instance_ret = get_call_ret_seq_bytes();
                    buff->addCodeToBuff(NULL, instance_ret->bytes, instance_ret->size);
                    
                    this->unresolved_calls.push_back(Entry(0, root->left->name, call_pos));
                    
                    break;
                }

                case Operator::RET: {
                    generate_all(root->left);
                            
                    byte_code_instance* instance = get_ret_seq_bytes();
                    buff->addCodeToBuff(NULL, instance->bytes, instance->size);
                    unresolved_returns.push_back(Entry(0, NULL, buff->bin_program_size() - IF_JMP_COND_MAGIC_OFFSET));
                    //there can be a kind of optimization
                    if (root->next != NULL) {
                        generate_all(root->next);
                    }
                    break;
                }

                GENERATE_BINARY_STANDART(ADD, add);
                GENERATE_BINARY_STANDART(SUB, sub);
                GENERATE_BINARY_STANDART(MUL, mul);
                GENERATE_BINARY_STANDART(DIV, div);
                GENERATE_BINARY_STANDART(MOD, mod);
                GENERATE_BINARY_STANDART(AND, and);
                GENERATE_BINARY_STANDART(OR, or);
                GENERATE_BINARY_STANDART(SHL, shl);
                GENERATE_BINARY_STANDART(SHR, shr);
                GENERATE_BINARY_STANDART(EQUAL, equal);
                GENERATE_BINARY_STANDART(NON_EQ, non_eq);
                GENERATE_BINARY_STANDART(LESS, less);
                GENERATE_BINARY_STANDART(GRTR, grtr);
                GENERATE_BINARY_STANDART(LESS_EQ, less_eq);
                GENERATE_BINARY_STANDART(GRTR_EQ, grtr_eq);

            

                break;
            };

            break;
        }
        
        default: {
            fprintf(stderr, "maaaan, there`s literally something wrong....");
            assert(0);
        }
        
    }
}


void FViewTable::process_calls(int this_offset, FViewTable& table_offsets) {
    for (size_t it = 0; it < this->unresolved_calls.size(); ++it) {
        Entry* func = table_offsets.findEntry(unresolved_calls[it].name);
        if (func == NULL) {
            fprintf(stderr, "cant find function %s\n", unresolved_calls[it].name);
            assert(0);
        }
        int mid = (unresolved_calls[it].offset + this_offset + IF_JMP_COND_MAGIC_OFFSET);
        this->buff->writeLittleEndianInt(unresolved_calls[it].offset, func->offset - mid);
    }

    for (size_t it = 0; it < this->childs.size(); ++it) {
        this->childs[it].process_calls(this_offset, table_offsets);
    }

    return;
}


void FViewTable::process_return_jmps(int return_begin) {
    for (size_t it = 0; it < unresolved_returns.size(); ++it) {
        buff->writeLittleEndianInt(unresolved_returns[it].offset, return_begin - unresolved_returns[it].offset - 4);
    }
    for (size_t it = 0; it < childs.size(); ++it) {
        childs[it].process_return_jmps(return_begin);
    }
    return;
}


//======================================================================================
// Code generation functions
//======================================================================================


/**
 * @brief Get endl sequence of bytes stored on return ptr value
 * 
 * @return char* - ptr on stored data 
 */
byte_code_instance* FViewTable::get_func_body_begin_seq_bytes() {
    //TODO:
    static unsigned char byte_arr[] = { 
        0x55,                                       // push   rbp
        0x48, 0x89, 0xE5,                           // mov    rbp,rsp
        0x48, 0x81, 0xC4, 0x10, 0x27, 0x00, 0x00    // add    rsp, 'number'
    };
    static byte_code_instance instance = {
        .bytes = byte_arr,
        .size = sizeof(byte_arr)
    };
    return &instance;
}

/**
 * @brief Get endl sequence of bytes stored on return ptr value
 * 
 * @return char* - ptr on stored data 
 */
byte_code_instance* FViewTable::get_func_body_end_seq_bytes() {
    //TODO:
    static unsigned char byte_arr[] = { 
        0x48, 0x81, 0xEC, 0x10, 0x27, 0x00, 0x00,   // add rsp, 'number'
        0x5D,                                       // pop rbp
        0xC3                                        // ret
    };
    static byte_code_instance instance = {
        .bytes = byte_arr,
        .size = sizeof(byte_arr)
    };
    return &instance;
}





/**
 * @brief Get endl sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* FViewTable::get_ret_seq(VarConst lhs, VarConst rhs) {
    //TODO:
    
}

/**
 * @brief Get endl sequence of bytes stored on return ptr value
 * 
 * @return char* - ptr on stored data 
 */
byte_code_instance* FViewTable::get_ret_seq_bytes() {
    //TODO:
    static unsigned char byte_arr[] = {
        0x58,                           // pop rax
        0xe9, 0x80, 0x01, 0x00, 0x00    // relative jmp on some big offset that will be replaced on return expression on the end of function   
    };
    static byte_code_instance instance = {
        .bytes = byte_arr,
        .size = sizeof(byte_arr)
    };
    return &instance;
}


/**
 * @brief Get endl sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* FViewTable::get_call_seq(VarConst lhs, VarConst rhs) {
    //TODO:
    
}

/**
 * @brief Get endl sequence of bytes stored on return ptr value
 * 
 * @return char* - ptr on stored data 
 */
byte_code_instance* FViewTable::get_call_seq_bytes() {
    //TODO:
    static unsigned char byte_arr[] = { 
        0xE8, 0x00, 0x00, 0x00, 0x00 //call function on offset (that needs to be changed)
    };
    static byte_code_instance instance = {
        .bytes = byte_arr,
        .size = sizeof(byte_arr)
    };
    return &instance;
}



/**
 * @brief Get endl sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* FViewTable::get_while_cond_seq(VarConst lhs, VarConst rhs) {
    //TODO:
    
}

/**
 * @brief Get endl sequence of bytes stored on return ptr value
 * 
 * @return char* - ptr on stored data 
 */
byte_code_instance* FViewTable::get_while_cond_seq_bytes() {
    //TODO:
    static unsigned char byte_arr[] = { 
        0x58,                               // pop rax
        0x48, 0x83, 0xF8, 0x00,             // cmp rax, 0x0
        0x0f, 0x84, 0x87, 0x65, 0x43, 0x21  // relative je on some big offset that will be replaced
    };
    static byte_code_instance instance = {
        .bytes = byte_arr,
        .size = sizeof(byte_arr)
    };
    return &instance;
}

/**
 * @brief Get endl sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* FViewTable::get_if_cond_seq(VarConst lhs, VarConst rhs) {
    //TODO:
    
}

/**
 * @brief Get endl sequence of bytes stored on return ptr value
 * 
 * @return char* - ptr on stored data 
 */
byte_code_instance* FViewTable::get_if_cond_seq_bytes() {
    //TODO:
    static unsigned char byte_arr[] = { 
        0x58,                               // pop rax
        0x48, 0x83, 0xF8, 0x00,             // cmp rax, 0x0
        0x0f, 0x84, 0x87, 0x65, 0x43, 0x21  // relative je on some big offset that will be replaced
    };
    static byte_code_instance instance = {
        .bytes = byte_arr,
        .size = sizeof(byte_arr)
    };
    return &instance;
}


/**
 * @brief Get endl sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* FViewTable::get_else_while_seq(VarConst lhs, VarConst rhs) {
    //TODO:
}

/**
 * @brief Get endl sequence of bytes stored on return ptr value
 * 
 * @return char* - ptr on stored data 
 */
byte_code_instance* FViewTable::get_else_while_seq_bytes() {
    //TODO:
    static unsigned char byte_arr[] = {
        0xe9, 0x80, 0x01, 0x00, 0x00 // relative jmp on some big offset that will be replaced     
    };
    static byte_code_instance instance = {
        .bytes = byte_arr,
        .size = sizeof(byte_arr)
    };
    return &instance;
}


/**
 * @brief Get endl sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* FViewTable::get_call_ret_seq(VarConst lhs, VarConst rhs) {
    //TODO:
}

/**
 * @brief Get endl sequence of bytes stored on return ptr value
 * 
 * @return char* - ptr on stored data 
 */
byte_code_instance* FViewTable::get_call_ret_seq_bytes() {
    //TODO:
    static unsigned char byte_arr[] = {
        0x50    // push rax
    };
    static byte_code_instance instance = {
        .bytes = byte_arr,
        .size = sizeof(byte_arr)
    };
    return &instance;
}


/**
 * @brief Get endl sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* FViewTable::get_endl_seq(VarConst lhs, VarConst rhs) {
    //TODO:
}

/**
 * @brief Get endl sequence of bytes stored on return ptr value
 * 
 * @return char* - ptr on stored data 
 */
byte_code_instance* FViewTable::get_endl_seq_bytes() {
    //TODO:
    static unsigned char byte_arr[] = {
        0x5b    // pop rbx
    };
    static byte_code_instance instance = {
        .bytes = byte_arr,
        .size = sizeof(byte_arr)
    };
    return &instance;
}

char* FViewTable::get_local_var_seq(int offset) {
    //TODO:
}

/**
 * @brief generate push of imm const
 * 
 * @param imm_const 
 * @return byte_code_instance* 
 */
byte_code_instance* FViewTable::get_local_var_seq_bytes(int offset) {
    static unsigned char byte_arr[] = {
        0x48, 0x8b, 0x85, 0x00, 0x00, 0x00, 0x00,   // mov rax, qword ptr [rbp +- offset]
        0x50                                        // push rax
    };

    //set offset
    // offset += 8;
    byte_arr[3] = (char)(offset & 0xff);
    byte_arr[4] = (char)((offset & 0xff00) >> 8);
    byte_arr[5] = (char)((offset & 0xff0000) >> 16);
    byte_arr[6] = (char)((offset & 0xff000000) >> 24);

    static byte_code_instance instance = {
        .bytes = byte_arr,
        .size = sizeof(byte_arr)
    };
    return &instance;

}


char* FViewTable::get_imm_const_seq(int imm_const) {
    //TODO:
}

/**
 * @brief generate push of imm const
 * 
 * @param imm_const 
 * @return byte_code_instance* 
 */
byte_code_instance* FViewTable::get_imm_const_seq_bytes(int imm_const) {
    static unsigned char byte_arr[] = {
        0x68, 0x00, 0x00, 0x00, 0x00 //push imm_const
    };

    //set offset
    byte_arr[1] = (char)(imm_const & 0xff);
    byte_arr[2] = (char)((imm_const & 0xff00) >> 8);
    byte_arr[3] = (char)((imm_const & 0xff0000) >> 16);
    byte_arr[4] = (char)((imm_const & 0xff000000) >> 24);

    static byte_code_instance instance = {
        .bytes = byte_arr,
        .size = sizeof(byte_arr)
    };
    return &instance;

}

/**
 * @brief Get add sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* FViewTable::get_int_to_bool_seq(VarConst lhs, VarConst rhs) {
    //TODO:
}

/**
 * @brief Get add sequence of bytes stored on return ptr value
 * 
 * @return char* - ptr on stored data 
 */
//TODO:
//TODO:
byte_code_instance* FViewTable::get_int_to_bool_seq_bytes() {
    //TODO:
    static unsigned char byte_arr[] = {
        0x5b, // pop rbx
        0x5a, // pop rdx
        0x48, 0x01, 0xd3, // add rbx,rdx
        0x53 // push rbx
    };
    static byte_code_instance instance = {
        .bytes = byte_arr,
        .size = sizeof(byte_arr)
    };
    return &instance;

}


/**
 * @brief Get add sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* FViewTable::get_add_seq(VarConst lhs, VarConst rhs) {
    //TODO:
}

/**
 * @brief Get add sequence of bytes stored on return ptr value
 * 
 * @return char* - ptr on stored data 
 */
byte_code_instance* FViewTable::get_add_seq_bytes() {
    //TODO:
    static unsigned char byte_arr[] = {
        0x5b, // pop rbx
        0x5a, // pop rdx
        0x48, 0x01, 0xd3, // add rbx,rdx
        0x53 // push rbx
    };
    static byte_code_instance instance = {
        .bytes = byte_arr,
        .size = sizeof(byte_arr)
    };
    return &instance;

}

/**
 * @brief Get sub assembler text stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* FViewTable::get_sub_seq(VarConst lhs, VarConst rhs) {
    //TODO:
}

/**
 * @brief Get sub sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
byte_code_instance* FViewTable::get_sub_seq_bytes() {
    //TODO:
    static unsigned char byte_arr[] = {
        0x5b, // pop rbx
        0x5a, // pop rdx
        0x48, 0x29, 0xd3, // sub rbx,rdx
        0x53 // push rbx
    };
    static byte_code_instance instance = {
        .bytes = byte_arr,
        .size = sizeof(byte_arr)
    };
    return &instance;
}

/**
 * @brief Get mul sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* FViewTable::get_mul_seq(VarConst lhs, VarConst rhs) {
    //TODO:
    
}

/**
 * @brief Get mul sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
byte_code_instance* FViewTable::get_mul_seq_bytes() {
    //TODO:
    static unsigned char byte_arr[] = {
        0x58, // pop rax
        0x5b, // pop rbx
        0x48, 0xf7, 0xeb, // imul rbx
        0x50 // push rax
    };
    static byte_code_instance instance = {
        .bytes = byte_arr,
        .size = sizeof(byte_arr)
    };
    return &instance;
}

/**
 * @brief Get div sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* FViewTable::get_div_seq(VarConst lhs, VarConst rhs) {
    //TODO:
}

/**
 * @brief Get div sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
byte_code_instance* FViewTable::get_div_seq_bytes() {
    //TODO:
    static unsigned char byte_arr[] = {
        0x41, 0x58, // pop r8
        0x58, // pop rax
        0x48, 0x31, 0xd2, // xor rdx,rdx
        0x49, 0xf7, 0xf8, // idiv r8
        0x50 // push rax
    };
    static byte_code_instance instance = {
        .bytes = byte_arr,
        .size = sizeof(byte_arr)
    };
    return &instance;
}

/**
 * @brief Get mod sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* FViewTable::get_mod_seq(VarConst lhs, VarConst rhs) {
    //TODO:
    
}

/**
 * @brief Get mod sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
byte_code_instance* FViewTable::get_mod_seq_bytes() {
    //TODO:
    static unsigned char byte_arr[] = {
        0x41, 0x58, // pop r8
        0x58, // pop rax
        0x48, 0x31, 0xd2, // xor rdx,rdx
        0x49, 0xf7, 0xf8, // idiv r8
        0x52 // push rdx
    };

    static byte_code_instance instance = {
        .bytes = byte_arr,
        .size = sizeof(byte_arr)
    };
    return &instance;
}

/**
 * @brief Get and sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* FViewTable::get_and_seq(VarConst lhs, VarConst rhs) {
    //TODO:
}

/**
 * @brief Get and sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
byte_code_instance* FViewTable::get_and_seq_bytes() {
    //TODO:
    static unsigned char byte_arr[] = {
        0x5b, // pop rbx
        0x5a, // pop rdx
        0x48, 0x21, 0xd3, // and rbx,rdx
        0x53 // push rbx
    };

    static byte_code_instance instance = {
        .bytes = byte_arr,
        .size = sizeof(byte_arr)
    };
    return &instance;
}

/**
 * @brief Get or sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* FViewTable::get_or_seq(VarConst lhs, VarConst rhs) {
    //TODO:
}

/**
 * @brief Get or sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
byte_code_instance* FViewTable::get_or_seq_bytes() {
    //TODO:
    static unsigned char byte_arr[] = {
        0x5b, // pop rbx
        0x5a, // pop rdx
        0x48, 0x09, 0xd3, // or rbx,rdx
        0x53 // push rbx
    };

    static byte_code_instance instance = {
        .bytes = byte_arr,
        .size = sizeof(byte_arr)
    };
    return &instance;
}

/**
 * @brief Get xor sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* FViewTable::get_xor_seq(VarConst lhs, VarConst rhs) {
    //TODO:
}

/**
 * @brief Get xor sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
byte_code_instance* FViewTable::get_xor_seq_bytes() {
    //TODO:
    static unsigned char byte_arr[] = {
        0x5b, // pop rbx
        0x5a, // pop rdx
        0x48, 0x31, 0xd3, // xor rbx,rdx
        0x53 // push rbx
    };

    static byte_code_instance instance = {
        .bytes = byte_arr,
        .size = sizeof(byte_arr)
    };
    return &instance;
}


/**
 * @brief Get shr sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* FViewTable::get_shl_seq(VarConst lhs, VarConst rhs) {
    //TODO:
    
}

/**
 * @brief Get shl sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
byte_code_instance* FViewTable::get_shl_seq_bytes() {
    //TODO:
    static unsigned char byte_arr[] = {
        0x59, // pop rcx
        0x5b, // pop rbx
        0x48, 0xd3, 0xe3, // shl rbx,cl
        0x53 // push rbx
    };

    static byte_code_instance instance = {
        .bytes = byte_arr,
        .size = sizeof(byte_arr)
    };
    return &instance;
}

/**
 * @brief Get shr sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* FViewTable::get_shr_seq(VarConst lhs, VarConst rhs) {
    //TODO:
}

/**
 * @brief Get shr sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
byte_code_instance* FViewTable::get_shr_seq_bytes() {
    //TODO:
    static unsigned char byte_arr[] = {
        0x59, // pop rcx
        0x5b, // pop rbx
        0x48, 0xd3, 0xeb, // shr rbx,cl
        0x53 // push rbx
    };

    static byte_code_instance instance = {
        .bytes = byte_arr,
        .size = sizeof(byte_arr)
    };
    return &instance;
}

/**
 * @brief Get set sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* FViewTable::get_set_seq(VarConst lhs) {
    //TODO:

}

/**
 * @brief Get set sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
byte_code_instance* FViewTable::get_set_seq_bytes(int offset) {
    //TODO:
    static unsigned char byte_arr[] = {
        0x58,                   //pop rax
                                //mov [rbp +- offset], rax
        0x48, 0x89, 0x85,       //mov byte seq for big offset and number
        0x40, 0xe2, 0x01, 0x00,  //int32 offset from rbp using little endian
        0x50
    };

    static byte_code_instance instance = {
        .bytes = byte_arr,
        .size = sizeof(byte_arr)
    };

    //set offset
    byte_arr[4] = (char)(offset & 0xff);
    byte_arr[5] = (char)((offset & 0xff00) >> 8);
    byte_arr[6] = (char)((offset & 0xff0000) >> 16);
    byte_arr[7] = (char)((offset & 0xff000000) >> 24);
    return &instance;

}

/**
 * @brief Get eq sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* FViewTable::get_eq_seq(VarConst lhs, VarConst rhs) {
    //TODO:
}

/**
 * @brief Get eq sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
byte_code_instance* FViewTable::get_equal_seq_bytes() {
    //TODO:
    static unsigned char byte_arr[] = {
        /*401004:*/         0x59,                      //pop    rcx
        /*401005:*/         0x5b,                      //pop    rbx
        /*401006:*/         0x48, 0x39, 0xd9,          //cmp    rcx,rbx
        /*401009:*/         0x74, 0x04,                //je     40100f <_start.if_true>
        /*40100b:*/         0x6a, 0x00,                //push   0x0
        /*40100d:*/         0x75, 0x02,                //jne    401011 <_start.if_end>

        //<_start.if_true>:
        /*40100f:*/         0x6a, 0x01                 //push   0x1

        //<_start.if_end>:
    };
    
    static byte_code_instance instance = {
        .bytes = byte_arr,
        .size = sizeof(byte_arr)
    };
    return &instance;

}

/**
 * @brief Get neq sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* FViewTable::get_non_eq_seq(VarConst lhs, VarConst rhs) {
    //TODO:
}

/**
 * @brief Get neq sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
byte_code_instance* FViewTable::get_non_eq_seq_bytes() {
    //TODO:
    static unsigned char byte_arr[] = {
        /*401004:*/         0x59,                      //pop    rcx
        /*401005:*/         0x5b,                      //pop    rbx
        /*401006:*/         0x48, 0x39, 0xd9,          //cmp    rcx,rbx
        /*401009:*/         0x75, 0x04,                //jne    40100f <_start.if_true>
        /*40100b:*/         0x6a, 0x00,                //push   0x0
        /*40100d:*/         0x74, 0x02,                //je     401011 <_start.if_end>

        //<_start.if_true>:
        /*40100f:*/         0x6a, 0x01                 //push   0x1

        //<_start.if_end>:
    };

    static byte_code_instance instance = {
        .bytes = byte_arr,
        .size = sizeof(byte_arr)
    };
    return &instance;
}

/**
 * @brief Get less sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* FViewTable::get_less_seq(VarConst lhs, VarConst rhs) {
    //TODO:
}

/**
 * @brief Get less sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
byte_code_instance* FViewTable::get_less_seq_bytes() {
    //TODO:
    static unsigned char byte_arr[] = {
        /*401004:*/         0x59,                      //pop    rcx
        /*401005:*/         0x5b,                      //pop    rbx
        /*401006:*/         0x48, 0x39, 0xd9,          //cmp    rcx,rbx
        /*401009:*/         0x7c, 0x04,                //jg     40100f <_start.if_true>
        /*40100b:*/         0x6a, 0x00,                //push   0x0
        /*40100d:*/         0x7d, 0x02,                //jle    401011 <_start.if_end>

        //<_start.if_true>:
        /*40100f:*/         0x6a, 0x01                 //push   0x1

        //<_start.if_end>:
    };

    static byte_code_instance instance = {
        .bytes = byte_arr,
        .size = sizeof(byte_arr)
    };
    return &instance;
}

/**
 * @brief Get grtr sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* FViewTable::get_grtr_seq(VarConst lhs, VarConst rhs) {
    //TODO:
}

/**
 * @brief Get grtr sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
byte_code_instance* FViewTable::get_grtr_seq_bytes() {
    //TODO:

    static unsigned char byte_arr[] = {
        /*401004:*/         0x59,                      //pop    rcx
        /*401005:*/         0x5b,                      //pop    rbx
        /*401006:*/         0x48, 0x39, 0xd9,          //cmp    rcx,rbx
        /*401009:*/         0x7f, 0x04,                //jg     40100f <_start.if_true>
        /*40100b:*/         0x6a, 0x00,                //push   0x0
        /*40100d:*/         0x7e, 0x02,                //jle    401011 <_start.if_end>

        //<_start.if_true>:
        /*40100f:*/         0x6a, 0x01                 //push   0x1

        //<_start.if_end>:
    };

    static byte_code_instance instance = {
        .bytes = byte_arr,
        .size = sizeof(byte_arr)
    };
    return &instance;

}

/**
 * @brief Get less eq sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* FViewTable::get_less_eq_seq(VarConst lhs, VarConst rhs) {
    //TODO:
}

/**
 * @brief Get less eq sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
byte_code_instance* FViewTable::get_less_eq_seq_bytes() {
    //TODO:
    static unsigned char byte_arr[] = {
        /*401004:*/         0x59,                      //pop    rcx
        /*401005:*/         0x5b,                      //pop    rbx
        /*401006:*/         0x48, 0x39, 0xd9,          //cmp    rcx,rbx
        /*401009:*/         0x7e, 0x04,                //jle     40100f <_start.if_true>
        /*40100b:*/         0x6a, 0x00,                //push   0x0
        /*40100d:*/         0x7f, 0x02,                //jg    401011 <_start.if_end>

        //<_start.if_true>:
        /*40100f:*/         0x6a, 0x01                 //push   0x1

        //<_start.if_end>:
    };

    static byte_code_instance instance = {
        .bytes = byte_arr,
        .size = sizeof(byte_arr)
    };
    return &instance;
}

/**
 * @brief Get grtr eq sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* FViewTable::get_grtr_eq_seq(VarConst lhs, VarConst rhs) {
    //TODO:
}

/**
 * @brief Get grtr or eq sequence of bytes
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
byte_code_instance* FViewTable::get_grtr_eq_seq_bytes() {
    //TODO:
    static unsigned char byte_arr[] = {
        /*401004:*/         0x59,                      //pop    rcx
        /*401005:*/         0x5b,                      //pop    rbx
        /*401006:*/         0x48, 0x39, 0xd9,          //cmp    rcx,rbx
        /*401009:*/         0x7d, 0x04,                //jge     40100f <_start.if_true>
        /*40100b:*/         0x6a, 0x00,                //push   0x0
        /*40100d:*/         0x7c, 0x02,                //jl    401011 <_start.if_end>

        //<_start.if_true>:
        /*40100f:*/         0x6a, 0x01                 //push   0x1

        //<_start.if_end>:
    };

    static byte_code_instance instance = {
        .bytes = byte_arr,
        .size = sizeof(byte_arr)
    };
    return &instance;
}

/**
 * @brief Get not sequence of asm commands
 * 
 * @param oper 
 * @return char* - ptr on stored data 
 */
char* FViewTable::get_not_seq(VarConst oper) {
    //TODO:
}

/**
 * @brief Get not sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
byte_code_instance* FViewTable::get_not_seq_bytes() {
    //TODO:
    static unsigned char byte_arr[] = {
        /*401005:*/         0x5b,                      //pop    rbx
        /*401006:*/         0x48, 0x83, 0xfb, 0x00,    //cmp    rbx, 0
        /*401009:*/         0x74, 0x04,                //je     40100f <_start.if_true>
        /*40100b:*/         0x6a, 0x00,                //push   0x0
        /*40100d:*/         0x75, 0x02,                //jl    401011 <_start.if_end>

        //<_start.if_true>:
        /*40100f:*/         0x6a, 0x01                 //push   0x1

        //<_start.if_end>:
    };

    static byte_code_instance instance = {
        .bytes = byte_arr,
        .size = sizeof(byte_arr)
    };
    return &instance;
}

/**
 * @brief Get inc sequence of asm commands
 * 
 * @param oper 
 * @return char* - ptr on stored data 
 */
char* FViewTable::get_inc_seq(VarConst oper) {
    //TODO:
}

/**
 * @brief Get inc sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */

unsigned char* FViewTable::get_inc_seq_bytes(VarConst lhs, VarConst rhs) {
    //TODO:
}

/**
 * @brief Get dec sequence of asm commands
 * 
 * @param oper 
 * @return char* - ptr on stored data 
 */
char* FViewTable::get_dec_seq(VarConst oper) {
    //TODO:
}

/**
 * @brief Get dec sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
unsigned char* FViewTable::get_dec_seq_bytes(VarConst lhs, VarConst rhs) {
    //TODO:
}