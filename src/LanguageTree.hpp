#ifndef LANGUAGE_TREE
#define LANGUAGE_TREE

#include <stdarg.h>
#include "LangTree.hpp"
#include "Tokenizer.hpp" 
#include "MyVector.h"

/**
 * @brief parsing functions through all program
 * 
 * @param lexer 
 * @return MyVector<Node*>& 
 */
Vector<Node*>& parse_functions(Lexer* lexer);


/**
 * @brief parsing function declaration
 * @details options: function "name"(args) {body} |  function "name"(args);
 * @param lexer 
 * @return Node* 
 */
Node* parse_func_decl(Lexer* lexer);

/**
 * @brief checking for obligatory tokens in function declaration
 * @param op_node 
 * @param refresh 
 * @return true 
 * @return false 
 */
bool parse_func_decl_require(Node* op_node, int& checker_iteration);



/**
 * @brief parsing variable declaration
 * @details options: var 'var_name' = 'value'
 * @details 'value' can be number or 
 * @param lexer 
 * @return Node* 
 */
Node* parse_decl_var(Lexer* lexer);


/**
 * @brief checking for obligatory tokens for decl_var
 * 
 * @param op_node 
 * @param refresh 
 * @return true 
 * @return false 
 */
bool parse_decl_var_require(Node* op_node, bool refresh); 


/**
 * @brief parsing function params
 * @param lexer 
 * @return Node* 
 */
Node* parse_func_params(Lexer* lexer);

/**
 * @brief checking for obligatory tokens for func_params
 * @details adding special "F_VIEW"
 * @param op_node 
 * @return true 
 * @return false 
 */
bool parse_func_params_require(Node* op_node);

/**
 * @brief parsing func_body: '{' [exprs]* '}'
 * 
 * @param lexer 
 * @return Node* 
 */
Node* parse_func_body(Lexer* lexer);

/**
 * @brief checking for obligatory tokens for func_body
 * @details '{' and '}'
 * @param  Node* op_node 
 * @return true 
 * @return false 
 */
bool parse_func_body_require(Node* op_node, int& checker_iteration);

/**
 * @brief parsing single expr: contains operations from numbers and variables 
 * @param lexer 
 * @return Node* 
 */
Node* parse_single_expr(Lexer* lexer);

/**
 * @brief checking for obligatory tokens for single_expr
 * @details now there is no obligatory tokens
 * @param op_node 
 * @return true 
 * @return false 
 */
bool  parse_single_expr_require(Node* op_node);

/**
 * @brief parsing multiple single_exprs,separated with comma and ending with endl(;)
 * 
 * @param lexer 
 * @return Node* 
 */
Node* parse_exprs(Lexer* lexer);

/**
 * @brief checking token if its sep ','
 * 
 * @param op_node 
 * @return true 
 * @return false 
 */
bool parse_exprs_require_sep(Node* op_node);

/**
 * @brief checking token if its sep ';'
 * 
 * @param op_node 
 * @return true 
 * @return false 
 */
bool parse_exprs_require_end(Node* op_node);


/**
 * @brief parsing while with syntax: while (single_expr) {"func_body"};
 * 
 * @param lexer 
 * @return Node* 
 */
Node* parse_while(Lexer* lexer);

/**
 * @brief checking obligatory tokens for while
 * @details tokens:'while', '(', ')', '{', '}'
 * @param Node* op_node 
 * @param refresh 
 * @return true 
 * @return false 
 */
bool parse_while_require(Node* op_node, int& checker_iteration);

/**
 * @brief parsing while with syntax: while (single_expr) {"func_body"};
 * 
 * @param lexer 
 * @return Node* 
 */
Node* parse_if(Lexer* lexer);

/**
 * @brief checking obligatory tokens for while
 * @details tokens:'if', '(', ')', func_body
 * @details optional: 'else', func_body
 * @param Node* op_node 
 * @param refresh 
 * @return true 
 * @return false 
 */
bool parse_if_require(Node* op_node, int& checker_iteration);


//TODO:
Node* parse_return(Lexer* lexer);
bool parse_return_require(Node* op_node, int& checker_iteration);


//TODO:
Node* parse_assigment_expr(Lexer* lexer);
bool  parse_assigment_expr_require(Node* op_node);

//TODO:
Node* parse_dereference(Lexer* lexer);
bool  parse_dereference_require(Node* op_node);

//TODO:
Node* parse_function_call(Lexer* lexer);
bool parse_function_call_require(Node* op_node, int& checker_iteration);

//TODO:
Node* parse_assigment_op(Lexer* lexer);
bool  parse_assigment_op_require(Node* op_node);

//TODO:
Node* parse_comparison(Lexer* lexer);
bool  parse_comparison_require(Node* op_node);

//TODO:
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


Node* parse_operators(Lexer* lexer);
bool  parse_operators_require(Node* op_node);

Node* parse_operator_operand(Lexer* lexer);
bool  parse_operator_operand_require(Node* op_node);

Node* parse_special_tern_op(Lexer* lexer);
bool  parse_special_tern_op_require(Node* op_node);

Node* parse_var_op(Lexer* lexer);
bool  parse_var_op_require(Node* op_node);


bool is_unary (Node* node);



#endif