#ifndef LANGUAGE_TREE
#define LANGUAGE_TREE

#include "LangTree.hpp"
#include "Tokenizer.hpp"

Node* parse_single_expr(Lexer* lexer);
Node* parse_single_expr_require(Lexer* lexer);

Node* parse_exprs(Lexer* lexer);
Node* parse_exprs_require(Lexer* lexer);

Node* parse_assigment_expr(Lexer* lexer);
Node* parse_assigment_expr_require(Lexer* lexer);


Node* parse_dereference(Lexer* lexer);
Node* parse_dereference_require(Lexer* lexer);


Node* parse_function_call(Lexer* lexer);
Node* parse_function_call_require(Lexer* lexer);

Node* parse_assigment_op(Lexer* lexer);
Node* parse_assigment_op_require(Lexer* lexer);

Node* parse_comparison(Lexer* lexer);
Node* parse_comparison_require(Lexer* lexer);

Node* parse_or(Lexer* lexer);
Node* parse_or_require(Lexer* lexer);

Node* parse_and(Lexer* lexer);
Node* parse_and_require(Lexer* lexer);

Node* parse_and(Lexer* lexer);
Node* parse_and_require(Lexer* lexer);

Node* parse_addsub(Lexer* lexer);
Node* parse_addsub_require(Lexer* lexer);

Node* parse_addsub(Lexer* lexer);
Node* parse_addsub_require(Lexer* lexer);

Node* parse_muldiv(Lexer* lexer);
Node* parse_muldiv_require(Lexer* lexer);

Node* parse_unary(Lexer* lexer);
Node* parse_unary_require(Lexer* lexer);

Node* parse_operand(Lexer* lexer);
Node* parse_operand_require(Lexer* lexer);

bool is_unary (Node* node);



#endif