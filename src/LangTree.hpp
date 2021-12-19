#include <inttypes.h>
#include <cstddef>

#ifndef STD_LANGTREE_H
#define STD_LANGTREE_H

#define STD_LANG_VERSION_MAJOR 1
#define STD_LAND_VERSION_MINOR 0

#define NODE_DEBUG_INFO

enum class NodeType{
    NONE       = 0,
    NUMBER     = 1,
    IDENTIFIER = 2,
    OPERATOR   = 3,
    CUSTOM     = 4,     // Only for temporary nodes. Not standart node type. 
};

#ifndef CUSTOM_NODE_DATA_TYPE
#define CUSTOM_NODE_DATA_TYPE int
#endif

/**
 * @warning Numeration of Operator can be changed on next versions. So do NOT relate on values.
 * @brief
 */

enum class Operator{
// ##### Incorrect oprerator #####
    NONE    = 0,

// ##### Arithm operators #####
    ADD     = 1,    // L + R  ok                                                          [basic]
    SUB     = 2,    // L - R  ok                                                        [basic]
    MUL     = 3,    // L * R  ok                                                          [basic]
    DIV     = 4,    // L / R  ok                                                          [basic]
    MOD     = 5,    // L % R  ok                                                          [arithm]
    AND     = 6,    // L & R  ok                                                          [arithm]
    OR      = 7,    // L | R  ok                                                          [arithm]
    XOR     = 8,    // L ^ R  ok                                                          [arithm]
    SHL     = 9,    // L << R ok                                                          [arithm]
    SHR     = 10,   // L >> R ok                                                          [arithm]

// ##### ASSIGNMENT #####
    SET     = 11,   // L := R (Set L value to R).                                       [basic]

// ##### COMPARE #####  
//!
//! @brief Return 0 or 1.
//!
    EQUAL   = 12,   // L == R ok                                                       [basic]
    NON_EQ  = 13,   // L != R  ok                                                          [basic]
    LESS    = 14,   // L < R  ok                                                           [basic]
    GRTR    = 15,   // L > R ok                                                            [basic]
    LESS_EQ = 16,   // L <= R ok                                                           [basic]
    GRTR_EQ = 17,   // L >= R ok                                                           [basic]

//  ##### UNARY #####
    NOT     = 18,  // !R ok                                                                [basic]
    INC     = 19,  // L++ or ++R  ok                                                       [unary]
    DEC     = 20,  // L-- or --R  ok                                                           [unary]

// ##### VAR & FUNC #####
    VAR     = 21,  // Declares L as local variable with R as value                        [basic]
    FUNC    = 22,  // Declares L as function with R-> as param tree (comma separated).    [basic]
    F_ARG   = 23,  // Declare function sepaerator. L is param tree. R is code             [basic]
    COMMA   = 24,  // Function  argument separator.                                      [multicall]
    CALL    = 25,  // Call L with R as paraments (comma separated).                     [basic]
    RET     = 26,  // Return from fuction. 

// ##### SEPARATORS #####
    ENDL    = 27, // L; R - if R returns R, else if L retunrs L else 0.                 [basic]
    QQ      = 28, // L ?? R - if L != 0 return L, else R                                [qq]
    TERN_Q  = 29, // L ? (R->L) : (R->R)  - if L return R->L else R->R                  [basic]
    TERN_C  = 30, // -----------^                                                       [basic]

// ##### LOOP #####
    WHILE   = 31, // while(L) R                                                         [basic]
// ? BREAK = 32,

// ##### MEMORY #####
    ADDR    = 33, // return adress of R(VAR).                                           [memory]
    VAL     = 34, // return value  of R. (From memory).                                 [memory]

// ##### DIFFERENTIATE #####
    DIFF    = 35, // (R)'. R must differable.                                           [basic]
};


typedef int      num_t;
typedef Operator opr_t;
typedef int      idt_t;

union NodeData
{
    num_t num;
    opr_t opr;
    idt_t  id;

    CUSTOM_NODE_DATA_TYPE custom;
};


struct Node
{
    NodeType type = NodeType::NONE;
    NodeData data = {0};

    Node* left  = 0;
    Node* right = 0;

    #ifdef NODE_DEBUG_INFO
    char*  name = 0;
    size_t line = 0;
    size_t pos  = 0;
    #endif 
};


#endif