#ifndef CODE_GENERATOR
#define CODE_GENERATOR

#include "LanguageTree.hpp"

union reference {
    int offset = 0;
    int value;
};

class VarConst {
    bool is_memory = false;
    reference ref;
    VarConst(bool is_mem, int val) : is_memory(is_mem), ref({val}) {};
};

/**
 * @brief Get add sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_add_seq(VarConst lhs, VarConst rhs);

/**
 * @brief Get add sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_add_seq_bytes(VarConst lhs, VarConst rhs);

/**
 * @brief Get sub assembler text stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_sub_seq(VarConst lhs, VarConst rhs);

/**
 * @brief Get sub sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_sub_seq_bytes(VarConst lhs, VarConst rhs);

/**
 * @brief Get mul sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_mul_seq(VarConst lhs, VarConst rhs);

/**
 * @brief Get mul sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_mul_seq_bytes(VarConst lhs, VarConst rhs);

/**
 * @brief Get div sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_div_seq(VarConst lhs, VarConst rhs);

/**
 * @brief Get div sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_div_seq_bytes(VarConst lhs, VarConst rhs);

/**
 * @brief Get mod sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_mod_seq(VarConst lhs, VarConst rhs);

/**
 * @brief Get mod sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_mod_seq_bytes(VarConst lhs, VarConst rhs);

/**
 * @brief Get and sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_and_seq(VarConst lhs, VarConst rhs);

/**
 * @brief Get and sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_and_seq_bytes(VarConst lhs, VarConst rhs);

/**
 * @brief Get or sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_or_seq(VarConst lhs, VarConst rhs);

/**
 * @brief Get or sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_or_seq_bytes(VarConst lhs, VarConst rhs);

/**
 * @brief Get xor sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_xor_seq(VarConst lhs, VarConst rhs);

/**
 * @brief Get xor sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_xor_seq_bytes(VarConst lhs, VarConst rhs);


/**
 * @brief Get shr sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_shl_seq(VarConst lhs, VarConst rhs);

/**
 * @brief Get shl sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_shl_seq_bytes(VarConst lhs, VarConst rhs);

/**
 * @brief Get shr sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_shr_seq(VarConst lhs, VarConst rhs);

/**
 * @brief Get shr sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_shr_seq_bytes(VarConst lhs, VarConst rhs);

/**
 * @brief Get set sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_set_seq(VarConst lhs, VarConst rhs);

/**
 * @brief Get set sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_set_seq_bytes(VarConst lhs, VarConst rhs);

/**
 * @brief Get eq sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_eq_seq(VarConst lhs, VarConst rhs);

/**
 * @brief Get eq sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_eq_seq_bytes(VarConst lhs, VarConst rhs);

/**
 * @brief Get neq sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_neq_seq(VarConst lhs, VarConst rhs);

/**
 * @brief Get neq sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_neq_seq_bytes(VarConst lhs, VarConst rhs);

/**
 * @brief Get less sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_less_seq(VarConst lhs, VarConst rhs);

/**
 * @brief Get less sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_less_seq_bytes(VarConst lhs, VarConst rhs);

/**
 * @brief Get grtr sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_grtr_seq(VarConst lhs, VarConst rhs);

/**
 * @brief Get grtr sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_grtr_seq_bytes(VarConst lhs, VarConst rhs);

/**
 * @brief Get less eq sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_less_eq_seq(VarConst lhs, VarConst rhs);

/**
 * @brief Get less eq sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_less_eq_seq_bytes(VarConst lhs, VarConst rhs);

/**
 * @brief Get grtr eq sequence of asm commands
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_grtr_eq_seq(VarConst lhs, VarConst rhs);

/**
 * @brief Get grtr or eq sequence of bytes
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_grtr_eq_seq_bytes(VarConst lhs, VarConst rhs);

/**
 * @brief Get not sequence of asm commands
 * 
 * @param oper 
 * @return char* - ptr on stored data 
 */
char* get_not_seq(VarConst oper);

/**
 * @brief Get not sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_not_seq_bytes(VarConst lhs, VarConst rhs);

/**
 * @brief Get inc sequence of asm commands
 * 
 * @param oper 
 * @return char* - ptr on stored data 
 */
char* get_inc_seq(VarConst oper);

/**
 * @brief Get inc sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_inc_seq_bytes(VarConst lhs, VarConst rhs);

/**
 * @brief Get dec sequence of asm commands
 * 
 * @param oper 
 * @return char* - ptr on stored data 
 */
char* get_dec_seq(VarConst oper);

/**
 * @brief Get dec sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_dec_seq_bytes(VarConst lhs, VarConst rhs);

/**
 * @brief Get not sequence of asm commands
 * 
 * @param oper 
 * @return char* - ptr on stored data 
 */
char* get_not_seq(VarConst oper);

/**
 * @brief Get not sequence of bytes stored on return ptr value
 * 
 * @param VarConst: lhs 
 * @param VarConst: rhs 
 * @return char* - ptr on stored data 
 */
char* get_not_seq_bytes(VarConst lhs, VarConst rhs);

#endif