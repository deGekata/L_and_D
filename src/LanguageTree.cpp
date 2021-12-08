#include "LanguageTree.hpp"


Node* parse_single_expr(Lexer* lexer) {
    assert(lexer && "lexer must not be null");

    // Node* token = get_node(lexer);
    
    // switch (token->type == NodeType::) {
    // case /* constant-expression */:
    //     /* code */
    //     break;
    
    // default:
    //     break;
    // }
    // if (token->type == NodeType::CUSTOM) {

    // }
    assert(0 && "TODO single expr");

}

Node* parse_single_expr_require(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
}

Node* parse_exprs(Lexer* lexer) {
    assert(lexer && "lexer must not be null");



}

Node* parse_exprs_require(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
}

Node* parse_assigment_expr(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
}

Node* parse_assigment_expr_require(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
}


Node* parse_dereference(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
}

Node* parse_dereference_require(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
}


Node* parse_function_call(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
}
Node* parse_function_call_require(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
}

Node* parse_assigment_op(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
}

Node* parse_assigment_op_require(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
}

Node* parse_comparison(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
}

Node* parse_comparison_require(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
}

Node* parse_or(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
}

Node* parse_or_require(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
}

Node* parse_xor(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
}

Node* parse_xor_require(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
}

Node* parse_and(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
}

Node* parse_and_require(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
}

Node* parse_addsub(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
    Node* ret_node = parse_muldiv(lexer);
    
    Node* op_node = get_node(lexer);
    
    if (op_node != NULL) {
        if (op_node->type == NodeType::OPERATOR) {
            if (op_node->data.opr == Operator::ADD || op_node->data.opr == Operator::SUB) {
                pop_node(lexer);
                op_node->left = ret_node;
                op_node->right = parse_addsub(lexer);
                return op_node;
            } else {
                //FIXME:
                assert(0 && "Expected MUL|DIV|MODE token");
            } 
        } else {
            //FIXME:
            assert(0 && "Expected MUL token");
        }
    }

    return ret_node;
}

Node* parse_addsub_require(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
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
    
    if (op_node != NULL) {
        if (op_node->type == NodeType::OPERATOR) {
            if (op_node->data.opr == Operator::MUL || op_node->data.opr == Operator::DIV || op_node->data.opr == Operator::MOD) {
                pop_node(lexer);
                op_node->left = ret_node;
                op_node->right = parse_muldiv(lexer);
                return op_node;
            }// else {
                //FIXME:
                // return op_node
            // } 
        } else {
            //FIXME:
            assert(0 && "Expected MUL token");
        }
    }

    return ret_node;
}

Node* parse_muldiv_require(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
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
        if (token->data.custom == ')') {/*raise error*/}
        if (token->data.custom == '(') {
            parse_single_expr(lexer);
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

