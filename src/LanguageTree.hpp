#ifndef LANGUAGE_TREE
#define LANGUAGE_TREE

#include <stdarg.h>
#include "LangTree.hpp"
#include "Tokenizer.hpp" 



Node* parse_single_expr(Lexer* lexer);
bool  parse_single_expr_require(Node* op_node);

Node* parse_exprs(Lexer* lexer);
bool  parse_exprs_require(Node* op_node);

Node* parse_assigment_expr(Lexer* lexer);
bool  parse_assigment_expr_require(Node* op_node);


Node* parse_dereference(Lexer* lexer);
bool  parse_dereference_require(Node* op_node);


Node* parse_function_call(Lexer* lexer);
bool parse_function_call_require(Node* op_node);

Node* parse_assigment_op(Lexer* lexer);
bool  parse_assigment_op_require(Node* op_node);


Node* parse_comparison(Lexer* lexer);
bool  parse_comparison_require(Node* op_node);

Node* parse_or(Lexer* lexer);
bool  parse_or_require(Node* op_node);

Node* parse_xor(Lexer* lexer);
bool  parse_xor_require(Node* op_node);

Node* parse_and(Lexer* lexer);
bool  parse_and_require(Node* op_node);

Node* parse_addsub(Lexer* lexer);
bool  parse_addsub_require(Node* op_node);

Node* parse_addsub(Lexer* lexer);
bool  parse_addsub_require(Node* op_node);

Node* parse_muldiv(Lexer* lexer);
bool  parse_muldiv_require(Node* op_node);

Node* parse_shift_op(Lexer* lexer);
bool  parse_shift_op_require(Node* op_node);

Node* parse_unary(Lexer* lexer);
Node* parse_unary_require(Lexer* lexer, Node* ret_node, Node* ret_node_dep);

Node* parse_operand(Lexer* lexer);
Node* parse_operand_require(Lexer* lexer);

Node* parse_special_tern_op(Lexer* lexer);
bool  parse_special_tern_op_require(Node* op_node);

Node* parse_var_op(Lexer* lexer);
bool  parse_var_op_require(Node* op_node);


bool is_unary (Node* node);



#endif