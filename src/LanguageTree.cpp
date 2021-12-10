#include "LanguageTree.hpp"

#define DO_BINARY_RECURSION(func_, additional_code) pop_node(lexer); op_node->left = ret_node; op_node->right = func_(lexer); return op_node;

#define NUMARGS(...)  (sizeof((Operator[]){__VA_ARGS__})/sizeof(Operator))
#define CHECK_EQ(ref, ...)  (check_eq(NUMARGS(__VA_ARGS__), ref, __VA_ARGS__))

#define REQUIRE_OP(op_, ...)                                                                        \
    if (op_node != NULL) {                                                                          \
        if (op_node->type == NodeType::OPERATOR) {                                                  \
            if (CHECK_EQ(op_, __VA_ARGS__)) return true;                                            \
        } else {                                                                                    \
            /*FIXME:*/                                                                              \
            printf("\033[1;30;41mOn line %d, column %d expected " # __VA_ARGS__ "\033[0m\n", op_node->line, op_node->pos);  \
            assert(0);                                                                              \
        }                                                                                           \
    }




bool check_eq(int cnt, Node* reference, ...) {
    assert(reference && "reference must not be NULL");

    bool ret_val = false;
    va_list ap;

    va_start(ap, cnt);

    while (cnt-- > 0) {
        ret_val |= reference->data.opr == va_arg(ap, Operator);
    }
    
    va_end(ap);

    return ret_val;
    
}


//TODO:
Node* parse_single_expr(Lexer* lexer) {
    assert(lexer && "lexer must not be null");

    Node* ret_node = parse_assigment_op(lexer);

    return ret_node;

    // assert(0 && "TODO single expr");

}

Node* parse_single_expr_require(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
}

//TODO:
Node* parse_exprs(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
}

Node* parse_exprs_require(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
}

//TODO:
Node* parse_assigment_expr(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
}

Node* parse_assigment_expr_require(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
}


//TODO:
Node* parse_dereference(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
}

Node* parse_dereference_require(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
}

//TODO:
Node* parse_function_call(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
}
Node* parse_function_call_require(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
}

Node* parse_assigment_op(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
    
    Node* ret_node = parse_comparison(lexer);
    Node* op_node = get_node(lexer);

    if (parse_assigment_op_require(op_node)) {
        DO_BINARY_RECURSION(parse_assigment_op, {
            if (ret_node->type == NodeType::OPERATOR && ret_node->data.opr == Operator::SHR)
        });        
    }
    return ret_node;
}

bool parse_assigment_op_require(Node* op_node) {
    REQUIRE_OP(op_node, Operator::SET);
}

//TODO:
Node* parse_comparison(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
    
    Node* ret_node = parse_or(lexer);
    Node* op_node = get_node(lexer);

    if (parse_comparison_require(op_node)) {
        DO_BINARY_RECURSION(parse_comparison, {});        
    }
    return ret_node;
}

bool parse_comparison_require(Node* op_node) {
    REQUIRE_OP(op_node, Operator::EQUAL, Operator::NON_EQ, Operator::GRTR, Operator::GRTR_EQ, Operator::LESS, Operator::LESS_EQ);
}

//TODO:
Node* parse_or(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
    Node* ret_node = parse_xor(lexer);
    
    Node* op_node = get_node(lexer);

    if (parse_or_require(op_node)) {
        DO_BINARY_RECURSION(parse_or, {});        
    }
    return ret_node;
}

bool parse_or_require(Node* op_node) {
    REQUIRE_OP(op_node, Operator::OR);
}

//TODO:
Node* parse_xor(Lexer* lexer) {
    assert(lexer && "lexer must not be null");

    Node* ret_node = parse_addsub(lexer);
    Node* op_node = get_node(lexer);

    if (parse_xor_require(op_node)) {
        DO_BINARY_RECURSION(parse_xor, {});        
    }
    return ret_node;
}

bool parse_xor_require(Node* op_node) {
    // assert(op_node && "lexer must not be null");

    REQUIRE_OP(op_node, Operator::XOR);

    return false;
}



//TODO:
Node* parse_and(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
    assert(lexer && "lexer must not be null");
    Node* ret_node = parse_addsub(lexer);
    
    Node* op_node = get_node(lexer);

    if (parse_and_require(op_node)) {
        DO_BINARY_RECURSION(parse_and, {});        
    }
    return ret_node;
}

bool parse_and_require(Node* op_node) {
    // assert(op_node && "lexer must not be null");

    REQUIRE_OP(op_node, Operator::AND);

    return false;
}

Node* parse_addsub(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
    Node* ret_node = parse_muldiv(lexer);
    
    Node* op_node = get_node(lexer);
    
    if (parse_addsub_require(op_node)) {
        DO_BINARY_RECURSION(parse_addsub, {});
    }

    return ret_node;
}

bool parse_addsub_require(Node* op_node) {
    // assert(op_node && "op_node must not be null");

    REQUIRE_OP(op_node, Operator::ADD, Operator::SUB);

    return false;
}

// Node* parse_addsub(Lexer* lexer) {
//     assert(lexer && "lexer must not be null");
// }

// Node* parse_addsub_require(Lexer* lexer) {
//     assert(lexer && "lexer must not be null");
// }


Node* parse_muldiv(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
    Node* ret_node = parse_unary(lexer);
    
    Node* op_node = get_node(lexer);
    
    if (parse_muldiv_require(op_node)) {
        DO_BINARY_RECURSION(parse_muldiv, {});
    }

    return ret_node;
}

bool parse_muldiv_require(Node* op_node) {
    // assert(lexer && "lexer must not be null");

    REQUIRE_OP(op_node, Operator::MUL, Operator::DIV, Operator::MOD);

    return false;

}

Node* parse_unary(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
    Node* ret_node = get_node(lexer);

    switch (ret_node->type) {
    case NodeType::OPERATOR:
        $deb
        if (is_unary(ret_node)) {
            $deb
            pop_node(lexer);
            Node* ret_node_dep =  parse_unary(lexer);
            printf("ret_node_dep  = %p\n", ret_node_dep);
            $deb
            if (ret_node->data.opr == Operator::INC || ret_node->data.opr == Operator::DEC) {
                $deb
                if (ret_node_dep->data.opr != Operator::INC &&
                    ret_node_dep->data.opr != Operator::DEC &&
                    ret_node_dep->type != NodeType::IDENTIFIER) {
                        $deb
                    printf("expected lvalue\n");
                    //TODO: raise error
                    assert(0);
                }
            }
            $deb
            ret_node->right = ret_node_dep;
            $deb
        } else {
            printf("raise error expected unary\n");
            //TODO: raise error
            assert(0);
        }
        break;
    
    default:
        $deb
        return parse_operand(lexer);
        break;
    }
    return ret_node;
}

Node* parse_unary_require(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
}

Node* parse_operand(Lexer* lexer) {
    assert(lexer && "lexer must not be null");

    Node* token = get_node(lexer);

    if (!token) return NULL;

    switch (token->type) {
    case NodeType::NUMBER:
        pop_node(lexer);
        return token;
        break;
    
    case NodeType::IDENTIFIER:
        pop_node(lexer);
        return token;
        break;
    
    
    case NodeType::CUSTOM:
        if (token->data.custom == ')') {return NULL;}
        if (token->data.custom == '(') {
            Node* ret_node = parse_single_expr(lexer);
            token = get_node(lexer);
            if (token == NULL || token->type != NodeType::CUSTOM || token->data.custom != ')' ) {
                assert(0 && "expextec closing paranthness");
                return NULL;
            }
        }
        break;

    default:
        break;
    }

}
Node* parse_operand_require(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
}

bool is_unary (Node* node) {
    assert(node && "null");

    return node->type == NodeType::OPERATOR && (
           node->data.opr == Operator::DEC || 
           node->data.opr == Operator::NOT || 
           node->data.opr == Operator::INC );
}

// Node* parse_number(Lexer* lexer) {
//     assert(lexer && "lexer must not be null");

//     Node* token = get_node(lexer);
//     if (token == NULL || token->type != NodeType::NUMBER) {
//         return NULL;
//     }
//     pop_node(lexer);
//     return token;
// }

// Node* parse_number_require(Lexer* lexer) {
//     assert(lexer && "lexer must not be null");
// }

