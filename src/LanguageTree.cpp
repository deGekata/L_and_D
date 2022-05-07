#include "LanguageTree.hpp"


int cur_offs = 0;

#define $fn for(int _my_it=0;_my_it<cur_offs;++_my_it){printf(" ");} printf("%d ", cur_offs); cur_offs++; printf("%s in func line = %d\n", __FUNCTION__, __LINE__);
#define $fn_ret(ret_val) {cur_offs--; return (ret_val);}

#define DO_BINARY_RECURSION(func_, additional_code) pop_node(lexer); op_node->left = ret_node; op_node->right = func_(lexer); return op_node;
#define DO_BINARY_RECURSION_FN(func_, additional_code) pop_node(lexer); op_node->left = ret_node; op_node->right = func_(lexer); $fn_ret(op_node);


#define NUMARGS(...)  (sizeof((Operator[]){__VA_ARGS__})/sizeof(Operator))
#define CHECK_EQ(ref, ...)  (check_eq(NUMARGS(__VA_ARGS__), ref, __VA_ARGS__))

#define REQUIRE_OP_RET(op_, ...)                                                                        \
    if (op_ != NULL) {\
        if (op_->type == NodeType::CUSTOM) {$fn_ret(false);}\
        if (op_->type == NodeType::OPERATOR) {                                                  \
            if (CHECK_EQ(op_, __VA_ARGS__)) $fn_ret(true);                                            \
        } else {                                                                                    \
            /*FIXME:*/                                                                              \
            printf("\033[1;30;41mOn line %d, column %d expected " # __VA_ARGS__ "\033[0m\n", op_->line, op_->pos);  \
            assert(0);                                                                              \
        }                                                                                           \
    }

#define REQUIRE_OP(op_, ...)                                                                        \
  ({if (op_ != NULL) {\
        if (op_->type == NodeType::CUSTOM) {false;}\
        if (op_->type == NodeType::OPERATOR) {                                                  \
            if (CHECK_EQ(op_, __VA_ARGS__)) {true;}                                            \
        } else {                                                                                    \
            /*FIXME:*/                                                                              \
            printf("\033[1;30;41mOn line %d, column %d expected " # __VA_ARGS__ "\033[0m\n", op_->line, op_->pos);  \
            false;                                                                              \
        }                                                                                           \
    } else {\
    false;}})

#define REQUIRE_CUSTOM_OP(op_, operator_) \
    ((!op_ || op_->type != NodeType::CUSTOM || op_->data.custom != operator_))


#define REQUIRE_CUSTOM_OP_RET(op_, operator_)\
  ({if (op_ != NULL) {\
        if (op_->type != NodeType::CUSTOM) {\
            $fn_ret(false);\
        } else {\
            if (op_->data.custom == operator_) {\
                $fn_ret(true);\
            } else {\
                $fn_ret(false);\
            }\
        }\
            } else {\
            $fn_ret(false);}\
        })


#define REQUIRE_NODE_TYPE(op_, type_) \
    ((op_ && op_->type == type_))



size_t print_offset = 0; 

#define $db for(int it = 0; it < print_offset; ++it) printf("  "); printf("%s in func line = %d\n", __FUNCTION__, __LINE__); print_offset++;


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

Node* parse_while(Lexer* lexer) {
    assert(lexer && "lexer must not be NULL");
    $fn

    Node* ret_node = get_node(lexer); 

    if (parse_while_require(ret_node)) {
        pop_node(lexer);

        Node* expr_node = get_node(lexer);
        if (REQUIRE_CUSTOM_OP(expr_node, '(')) {
            assert(0 && "expected ( after while keyword");
        }
        pop_node(lexer);

        expr_node = parse_single_expr(lexer);

    } else {
        $fn_ret(NULL);
    }

    $fn_ret(NULL);
}

bool parse_while_require(Node* op_node) {
    $fn
    REQUIRE_OP_RET(op_node, Operator::WHILE);
    
    $fn_ret(false);
}

Node* parse_func_decl(Lexer* lexer) {
    assert(lexer && "lexer must not be NULL");
    $fn

    parse_func_decl_require(NULL, true);

    Node* func_dec = get_node(lexer);
    // $deb
    printf("lool");
    if (! parse_func_decl_require(func_dec, false)) $fn_ret(NULL);
    pop_node(lexer);
    // $deb
    printf("lool");
    Node* func_name = get_node(lexer);
    if (! parse_func_decl_require(func_name, false)) $fn_ret(NULL);
    pop_node(lexer);
    // $deb
    Node* cur_node = get_node(lexer);
    if (! parse_func_decl_require(cur_node, false)) $fn_ret(NULL);
    pop_node(lexer);
    // $deb
    Node* func_params = parse_func_params(lexer);
    // $deb
    cur_node = get_node(lexer);
    if (! parse_func_decl_require(cur_node, false)) $fn_ret(NULL);
    pop_node(lexer);
    // $deb
    cur_node = get_node(lexer);
    if (! parse_func_decl_require(cur_node, false)) $fn_ret(NULL);
    pop_node(lexer);
    // $deb
    cur_node = parse_func_body(lexer);

    // return cur_node;
    // $deb
    printf("%x func params\n");
    func_dec->left = func_name;
    func_dec->right = (Node*) calloc(1, sizeof(Node));
    func_dec->right->type = NodeType::OPERATOR;
    func_dec->right->data.opr = Operator::F_ARG;
    func_dec->right->left  = func_params;
    func_dec->right->right = cur_node;
    // $deb
    cur_node = get_node(lexer);
    // printf("curnode->custom = %c\n",cur_node->data.custom);
    if (! parse_func_decl_require(cur_node, false)) $fn_ret(NULL);
    pop_node(lexer);
    // $deb
    printf("%d func dec????\n", func_dec);
    $fn_ret(func_dec);
    // return func_params;

}

bool parse_func_decl_require(Node* op_node, bool refresh) {
    //FIXME: hz zachem?
    $fn
    static int checker_iteration = 0;
    $deb
    if (refresh) { checker_iteration = 0; $fn_ret(false);}
$deb
    if (!op_node) $fn_ret(false);
$deb
    switch (checker_iteration) {
    
    case 0: {
        $deb
        checker_iteration = 1;
        REQUIRE_OP_RET(op_node, Operator::FUNC);
        printf("false ret");
        $fn_ret(false);
        break;
    }
    
    case 1: {
        checker_iteration = 2;
        printf("1--\n");
        if (op_node->type == NodeType::IDENTIFIER) $fn_ret(true);
        $fn_ret(false);
        break;
    }
        
    case 2: {
        checker_iteration = 3;
        printf("2--\n");
        REQUIRE_CUSTOM_OP_RET(op_node, '(');
        $fn_ret(false);
        break;
    }
        
    case 3: {
        checker_iteration = 4;
        printf("3--\n");
        REQUIRE_CUSTOM_OP_RET(op_node, ')');
        $fn_ret(false);
        break;
    }

    case 4: {
        printf("4--\n");
        checker_iteration = 5;
        REQUIRE_CUSTOM_OP_RET(op_node, '{');
        $fn_ret(false);
        break;
    }

    case 5: {
        printf("5--\n");
        checker_iteration = 0;
        REQUIRE_CUSTOM_OP_RET(op_node, '}');
        $fn_ret(false);
        break;
    }


    default:
        break;
    }
}   

//TODO: change single expr to single var
Node* parse_func_params(Lexer* lexer) {
    assert(lexer && "lexer must not be null");

    $fn

    Node* ret_node = parse_decl_var(lexer);
    Node* exp_node, *cur_node, *sep_node;
    exp_node = cur_node = sep_node = NULL;

    if (ret_node == NULL) {
        $fn_ret(NULL);
    }

    sep_node = get_node(lexer);

    if (parse_func_params_require(sep_node)) {
        cur_node = (Node*) calloc(1, sizeof(Node));
        exp_node = ret_node;
        ret_node = cur_node;
        do {
            // printf("%d exp_node ---------------------------------------------------\n", exp_node);
            printf("in do whille-----------------------------------------------\n");
            pop_node(lexer);
            cur_node->right = sep_node;
            sep_node->left = exp_node;
            exp_node = parse_decl_var(lexer);
            if (exp_node == NULL) {
                assert(0 && "expected expression, found ','");
            }
            cur_node = sep_node;
            sep_node = get_node(lexer);
        } while  (parse_func_params_require(sep_node));
        cur_node->right = exp_node;
        cur_node = ret_node;
        ret_node = ret_node->right;
        free(cur_node);
    }
    // cur_node->left = ret_node;
    // cur_node = get_node(lexer);
    // if (parse_exprs_require_end(cur_node)) {
    //     return cur_node;
    // } else {
    //     assert(0 && "expected ';'");
    // }
    
    $fn_ret(ret_node);
}

bool parse_func_params_require(Node* op_node) {
    $fn
    // assert(op_node && "op_node must not be NULL");

    REQUIRE_OP_RET(op_node, Operator::COMMA);

    $fn_ret(false);
}

Node* parse_func_body(Lexer* lexer) {
    assert(lexer && "lexer must not be NULL");
    $fn
    Node* root_node= parse_exprs(lexer);
    // Node* root_node1= parse_exprs(lexer);
    // Node* root_node2= parse_exprs(lexer);
    // Node* root_node3= parse_exprs(lexer);
    // Node* root_node4= parse_exprs(lexer);
    Node* cur_node = root_node;

    if (!root_node) $fn_ret(NULL);

    cur_node->right = parse_exprs(lexer);

    int it = 0;
    while (cur_node->right) {
        cur_node = cur_node->right;
        cur_node->right = parse_exprs(lexer);
        if (cur_node->right == NULL) cur_node->right = parse_operators(lexer);
        // if(cur_node->right->type == NodeType::CUSTOM) cur_node->right = NULL;
    }
    // cur_node->right = NULL;
    $fn_ret(root_node);    
}

bool parse_func_body_require(Node* op_node);

//TODO:
Node* parse_exprs(Lexer* lexer) {
    assert(lexer && "lexer must not be null");
    $fn
    Node* ret_node = parse_single_expr(lexer);
    Node* exp_node, *cur_node, *sep_node;
    exp_node = cur_node = sep_node = NULL;

    if (ret_node == NULL) {
        cur_node = get_node(lexer);
        if (cur_node != NULL && parse_exprs_require_end(cur_node)) {
            //TODO:
            $fn_ret(NULL);
            /*
             $fn_ret(cur_node);
            */
        } else {
            $fn_ret(NULL);
        }
    }

    sep_node = get_node(lexer);

    if (parse_exprs_require_sep(sep_node)) {
        cur_node = (Node*) calloc(1, sizeof(Node));
        exp_node = ret_node;
        ret_node = cur_node;
        do {
            // printf("%d exp_node ---------------------------------------------------\n", exp_node);
            printf("in do whille-----------------------------------------------\n");
            pop_node(lexer);
            cur_node->right = sep_node;
            sep_node->left = exp_node;
            exp_node = parse_single_expr(lexer);
            if (exp_node == NULL) {
                assert(0 && "expected expression, found ','");
            }
            cur_node = sep_node;
            sep_node = get_node(lexer);
        } while  (parse_exprs_require_sep(sep_node));
        cur_node->right = exp_node;
        cur_node = ret_node;
        ret_node = ret_node->right;
        free(cur_node);
    }
    cur_node = get_node(lexer);
    if (parse_exprs_require_end(cur_node)) {
        pop_node(lexer);
        cur_node->left = ret_node;
        // $fn_ret(cur_node);
        $fn_ret(cur_node);
    } else {
        assert(0 && "expected ';'");
    }
}

bool parse_exprs_require_sep(Node* op_node) {
    assert(op_node && "op_node must not be null");
    $fn
    REQUIRE_OP_RET(op_node, Operator::COMMA);
    
    $fn_ret(false);
}

bool parse_exprs_require_end(Node* op_node) {
    assert(op_node && "op_node must not be null");
    $fn
    REQUIRE_OP_RET(op_node, Operator::ENDL); 
    
    $fn_ret(false)
}

Node* parse_decl_var(Lexer* lexer) {
    assert(lexer && "lexer must not be NULL");
    $fn

    parse_decl_var_require(NULL, true);

    Node* decl_node = get_node(lexer);
    if (!parse_decl_var_require(decl_node, false)) {
        $fn_ret(NULL);
    }
    pop_node(lexer);

    decl_node->left = get_node(lexer);
    if (!parse_decl_var_require(decl_node->left, false)) {
        assert(0 && "found variable declaration, expected identifier");
    }
    pop_node(lexer);

    $fn_ret(decl_node);

}

bool parse_decl_var_require(Node* op_node, bool refresh) {
    $fn 
    static int state = 0;
    if (refresh) { state = 0; $fn_ret(false);}

    if (!op_node) {
        $fn_ret(false)
    }

    switch (state) {

    case 0: {
        state = 1;
        REQUIRE_OP_RET(op_node, Operator::VAR);
        break;
    }

    case 1: {
        state = 0;
        bool my_ret = REQUIRE_NODE_TYPE(op_node, NodeType::IDENTIFIER);
        $fn_ret(my_ret);
        break;
    }
    
    default:
        break;
    }


    $fn_ret(false)
}

//TODO:

Node* parse_single_expr(Lexer* lexer) {

    $fn
    assert(lexer && "lexer must not be null");

    Node* ret_node = parse_assigment_op(lexer);
    printf("%x single_ret\n", ret_node);
    $fn_ret(ret_node);

    // assert(0 && "TODO single expr");

}

Node* parse_single_expr_require(Lexer* lexer) {
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
    $fn
    assert(lexer && "lexer must not be null");
    
    Node* ret_node = parse_comparison(lexer);

    if (ret_node == NULL) {
        ret_node = parse_decl_var(lexer);
    }

    Node* op_node = get_node(lexer);

    if (parse_assigment_op_require(op_node)) {
        DO_BINARY_RECURSION_FN(parse_assigment_op, {
            if (ret_node->type == NodeType::OPERATOR && ret_node->data.opr == Operator::SHR)
        });        
    }

    
    $fn_ret(ret_node);
}

bool parse_assigment_op_require(Node* op_node) {
    $fn
    REQUIRE_OP_RET(op_node, Operator::SET);
    $fn_ret(false);
}

//TODO:
Node* parse_comparison(Lexer* lexer) {
    $fn
    assert(lexer && "lexer must not be null");
    
    Node* ret_node = parse_or(lexer);
    Node* op_node = get_node(lexer);

    if (parse_comparison_require(op_node)) {
        DO_BINARY_RECURSION_FN(parse_comparison, {});        
    }
    $fn_ret(ret_node);
}

bool parse_comparison_require(Node* op_node) {
    $fn
    REQUIRE_OP_RET(op_node, Operator::EQUAL, Operator::NON_EQ, Operator::GRTR, Operator::GRTR_EQ, Operator::LESS, Operator::LESS_EQ);
    $fn_ret(false);
}

//TODO:
Node* parse_or(Lexer* lexer) {
    $fn
    assert(lexer && "lexer must not be null");
    Node* ret_node = parse_xor(lexer);
    
    Node* op_node = get_node(lexer);

    if (parse_or_require(op_node)) {
        DO_BINARY_RECURSION_FN(parse_or, {});        
    }
    $fn_ret(ret_node);
}

bool parse_or_require(Node* op_node) {
    $fn
    REQUIRE_OP_RET(op_node, Operator::OR);
    $fn_ret(false);
}

//TODO:
Node* parse_xor(Lexer* lexer) {
    $fn
    assert(lexer && "lexer must not be null");

    Node* ret_node = parse_addsub(lexer);
    Node* op_node = get_node(lexer);

    if (parse_xor_require(op_node)) {
        DO_BINARY_RECURSION_FN(parse_xor, {});        
    }
    $fn_ret(ret_node);
}

bool parse_xor_require(Node* op_node) {
    $fn
    // assert(op_node && "lexer must not be null");

    REQUIRE_OP_RET(op_node, Operator::XOR);

    $fn_ret(false);
}



//TODO:
Node* parse_and(Lexer* lexer) {
    $fn
    assert(lexer && "lexer must not be null");
    assert(lexer && "lexer must not be null");
    Node* ret_node = parse_addsub(lexer);
    
    Node* op_node = get_node(lexer);

    if (parse_and_require(op_node)) {
        DO_BINARY_RECURSION_FN(parse_and, {});        
    }
    $fn_ret(ret_node);
}

bool parse_and_require(Node* op_node) {
    $fn
    // assert(op_node && "lexer must not be null");

    REQUIRE_OP_RET(op_node, Operator::AND);

    $fn_ret(false);
}

Node* parse_addsub(Lexer* lexer) {
    $fn
    assert(lexer && "lexer must not be null");
    // Node* ret_node = parse_operand(lexer);
    Node* ret_node = parse_muldiv(lexer);
    
    Node* op_node = get_node(lexer);
    
    if (parse_addsub_require(op_node)) {
        pop_node(lexer); 
        op_node->left = ret_node; 
        op_node->right = parse_addsub(lexer); 
        $fn_ret(op_node);
        // DO_BINARY_RECURSION_FN(parse_addsub, {});
    }

    $fn_ret(ret_node);
}

bool parse_addsub_require(Node* op_node) {
    $fn
    // assert(op_node && "op_node must not be null");

    REQUIRE_OP_RET(op_node, Operator::ADD, Operator::SUB);

    $fn_ret(false);
}

// Node* parse_addsub(Lexer* lexer) {
//     assert(lexer && "lexer must not be null");
// }

// Node* parse_addsub_require(Lexer* lexer) {
//     assert(lexer && "lexer must not be null");
// }


Node* parse_muldiv(Lexer* lexer) {
    print_offset++; 
    $fn
    assert(lexer && "lexer must not be null");
    Node* ret_node = parse_shift_op(lexer);
    
    Node* op_node = get_node(lexer);
    
    if (parse_muldiv_require(op_node)) {
        DO_BINARY_RECURSION_FN(parse_muldiv, {});
    }

    $fn_ret(ret_node);
}

bool parse_muldiv_require(Node* op_node) {
    $fn
    // assert(lexer && "lexer must not be null");

    REQUIRE_OP_RET(op_node, Operator::MUL, Operator::DIV, Operator::MOD);

    $fn_ret(false);

}

Node* parse_shift_op(Lexer* lexer) {
    $fn
    assert(lexer && "lexer must not be null");
    Node* ret_node = parse_special_tern_op(lexer);
    
    Node* op_node = get_node(lexer);
    
    if (parse_shift_op_require(op_node)) {
        DO_BINARY_RECURSION_FN(parse_shift_op, {});
    }

    $fn_ret(ret_node);
}

bool parse_shift_op_require(Node* op_node) {
    $fn
    // assert(lexer && "lexer must not be null");

    REQUIRE_OP_RET(op_node, Operator::SHL, Operator::SHR);

    $fn_ret(false);
}

Node* parse_special_tern_op(Lexer* lexer) {
    $fn
        // printf("lollolol");

    assert(lexer && "lexer must not be null");
    Node* ret_node = parse_unary(lexer);
    
    Node* op_node = get_node(lexer);
    
    if (parse_special_tern_op_require(op_node)) {
        printf("lollolol");
        DO_BINARY_RECURSION_FN(parse_special_tern_op, {});
    }

    $fn_ret(ret_node);
}

bool  parse_special_tern_op_require(Node* op_node) {
    $fn
    // assert(lexer && "lexer must not be null");

    REQUIRE_OP_RET(op_node, Operator::QQ);

    $fn_ret(false);
}

Node* parse_unary(Lexer* lexer) {
    $fn
    assert(lexer && "lexer must not be null");
    Node* ret_node = get_node(lexer);
    if (ret_node == NULL) $fn_ret(NULL);
    switch (ret_node->type) {
    case NodeType::OPERATOR:
        $deb
        if (is_unary(ret_node)) {
            $deb
            pop_node(lexer);
            Node* ret_node_dep =  parse_unary(lexer);
            printf("ret_node_dep  = %p\n", ret_node_dep);
            $deb
            parse_unary_require(lexer, ret_node, ret_node_dep);
            $deb
            ret_node->right = ret_node_dep;
            $deb
        } else {
            printf("raise error expected unary\n");
            //TODO: raise error
            $fn_ret(NULL);
        }
        break;
    
    default:
        $deb
        Node* tmp_node = parse_operand(lexer);
        $fn_ret(tmp_node);
        break;
    }
    $fn_ret(ret_node);
}

Node* parse_unary_require(Lexer* lexer, Node* ret_node, Node* ret_node_dep) {
    assert(lexer && "lexer must not be null");
    if (ret_node->data.opr == Operator::INC || ret_node->data.opr == Operator::DEC) {
        $deb

        if (ret_node_dep->data.opr != Operator::INC &&
            ret_node_dep->data.opr != Operator::DEC &&
            ret_node_dep->type != NodeType::IDENTIFIER) 
        {
            $deb
            // &*&*&*&*&*&*ret_node_dep;
            printf("expected lvalue\n");
            //TODO: raise error
            assert(0);
        }
    }
}

Node* parse_operand(Lexer* lexer) {
    $fn
    assert(lexer && "lexer must not be null");

    Node* token = get_node(lexer);

    if (!token) $fn_ret(NULL);

    switch (token->type) {
    case NodeType::NUMBER :
        pop_node(lexer);
        $fn_ret(token);
        break;
    
    case NodeType::IDENTIFIER:
        pop_node(lexer);
        $fn_ret(token);
        break;
    
    
    case NodeType::CUSTOM:
        printf("custooom");
        // if (token->data.custom == ')') {return NULL;}
        if (token->data.custom == '(') {
            printf("(((");
            pop_node(lexer);
            Node* ret_node = parse_single_expr(lexer);
            printf("after single expr-----------\n");
            token = get_node(lexer);
            if (token == NULL || token->type != NodeType::CUSTOM || token->data.custom != ')' ) {
                assert(0 && "expextec closing paranthness");
                $fn_ret(NULL);
            }
            pop_node(lexer);
            $fn_ret(ret_node);
        }
        break;

    case NodeType::OPERATOR: {
        printf("operator operand---------\n");  
        Node* ret_node = parse_operator_operand(lexer);
        break;
    }

    default:
        {break;}
    }

    $fn_ret(NULL);
}
Node* parse_operand_require(Lexer* lexer) {
    $fn
    assert(lexer && "lexer must not be null");
    $fn_ret(NULL);
}



Node* parse_operator_operand(Lexer* lexer) {

}

bool parse_operator_operand(Node* op_node) {

}

Node* parse_var_op(Lexer* lexer) {
    assert(lexer && "lexer must not be null");

    Node* op_node = get_node(lexer);

    if (parse_var_op_require(op_node)) {
        pop_node(lexer);
        Node* var_name = get_node(lexer);
        
    }
}

bool  parse_var_op_require(Node* op_node) {
    $fn
    REQUIRE_OP_RET(op_node, Operator::VAR);
}


bool is_unary (Node* node) {
    assert(node && "null");

    return node->type == NodeType::OPERATOR && (
           node->data.opr == Operator::DEC || 
           node->data.opr == Operator::NOT || 
           node->data.opr == Operator::INC );
}



