#include "Semantic_check.hpp"

ProgramBuff::~ProgramBuff() {
    for (size_t it = 0; it < progam.size(); ++it)
        free(progam[it]);
    // progam.~MyVector();
    // binary_program.~MyVector();
}


//TODO: add bytes to binary program
void ProgramBuff::addCodeToBuff(char* listing, char* bytes, size_t bytes_len) {
    if (listing == NULL) {
        return;
    }
    progam.push_back(listing);

    return;
}


// FViewTable& FViewTable::operator=(const FViewTable& rhs) {
//     this->buff = rhs.buff;

//     return *this;
// }

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
    FViewTable* temp_table = new FViewTable;
    func_tables.push_back(temp_table);
    members.push_back(Entry(0, name, offset));
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




void generate_code(FILE* file) {
    
    Lexer* lexer = (Lexer*) calloc(1, sizeof(Lexer));
    init_lexer(lexer, file);
    init_operator_hashes();
    
    MyVector<Node*> functions = parse_functions(lexer);
    FViewTable functions_table = FViewTable();

    // Node* node = parse_func_decl(lexer);
    // if (node == NULL && get_node(lexer) != NULL) assert(0 && "error while parsing tree"); 
    // printf("node is %d\n", node);
    MyVector<FViewTable> func_tables;

    for (size_t it = 0; it < functions.size(); ++it) {
        bool is_not_ok = functions_table.insertEntry(functions[it]->left->name);
        FViewTable* temp_table = new FViewTable;
        func_tables.push_back(temp_table);
        if (is_not_ok) {
            fprintf(stderr, "found duplication in functions name");
            assert(0);
        }
        draw_tree(functions[it]);
    }

    for (size_t it = 0; it < func_tables.size(); ++it) {
        generate_func_params(functions[it], func_tables[it]);
    }


    // FViewTable main_table;
    1 + 4;
    return;

}

void generate_func_params(Node* node, FViewTable& table) {
    Node* cur_node = node->right->left;
    if (cur_node == NULL) return;

    int64_t max_offset = -24;  //return seg + return offset + pushed rbp

    if (cur_node->type == NodeType::OPERATOR) {
        while (cur_node->type == NodeType::OPERATOR) {
            max_offset -= 8;
            cur_node = cur_node->right;
        }

        cur_node = node->right->left;

        while (cur_node->type == NodeType::OPERATOR) {
            bool is_not_ok = table.insertEntry(cur_node->left->name, max_offset);
            if (is_not_ok) {
                fprintf(stderr, "found duplication in variables");
            }
            max_offset += 8;
            cur_node = cur_node->right;
        }
    }

    bool is_not_ok = table.insertEntry(cur_node->name, max_offset);
    if (is_not_ok) {
        fprintf(stderr, "found duplication in variables");
    }
    // table.insertEntry(cur_node->name, max_offset);

    
    return;
}

void check_func_params(FViewTable& functions_table, FViewTable& func_table) {
    for (size_t it_var = 0; it_var < func_table.members.size(); ++it_var) {
        for (size_t it_func = 0; it_func < functions_table.members.size(); ++it_func) {
            if (strcmp(functions_table.members[it_func].name, func_table.members[it_var].name) == 0) {
                fprintf(stderr, "found collision of function param and function name: function '%s' and variable '%s'\n", functions_table.members[it_func].name, func_table.members[it_var].name);
                assert(0);
            }
        }
    }
}


void generate_func_body(Node* node, FViewTable& table) {
    
}
