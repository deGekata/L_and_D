#ifndef SEMANTIC_CHECK
#define SEMANTIC_CHECK

#include "MyVector.h"
#include "LanguageTree.hpp"
#include "CodeGenerator.hpp"
#include "LangTreeDraw.hpp"

#define IF_JMP_COND_MAGIC_OFFSET 4

union reference {
    int offset = 0;
    int value;
};

class VarConst {
public:
    bool is_memory = false;
    reference ref;
    VarConst(bool is_mem, int val) : is_memory(is_mem), ref({val}) {};
};

struct byte_code_instance {
    unsigned char* bytes = NULL;
    size_t size = 0;
};


class ProgramBuff {
public:
    void addCodeToBuff(char* listing, unsigned char* bytes, size_t bytes_len);
    void writeLittleEndianInt(size_t offset, int value);
    size_t bin_program_size() const;
    
    ~ProgramBuff();
    void dropBuffToFile(FILE* file);
private:
    Vector<unsigned char> binary_program;
    Vector<char*> program;
};

class Entry {
public:

    size_t id = 0;
    char* name = NULL;
    int64_t offset = 0;
    Entry() = default;
    Entry(size_t _id, char* _name) : id(_id), name(_name) {};
    Entry(size_t _id, char* _name, int64_t _offset) : id(_id), name(_name), offset(_offset) {};
    // Entry& operator=(const Entry&) = default;
    // Entry& operator=(Entry&&) = default;
    // Entry(Entry&) = default;
    // Entry(Entry&&) = default;
    
    ~Entry() {};
};

class FViewTable {
public:
    ProgramBuff* buff = NULL;
    FViewTable* parent = NULL;
    Vector<FViewTable> childs;
    Vector<Entry> members;
    Vector<Entry> unresolved_calls;
    Vector<Entry> unresolved_returns;
    
    int current_size = 0;
    int table_offset = 0;
    int max_child_size = 0;
    int global_code_offset = 0;

    FViewTable() {};
    FViewTable(FViewTable* parent, ProgramBuff* global_buff, int _table_offset) : parent(parent), buff(global_buff), table_offset(_table_offset) {};
    FViewTable(FViewTable&& temporary) {
        this->current_size = temporary.current_size;
        this->table_offset = temporary.table_offset;
        this->max_child_size = temporary.max_child_size;
        this->buff = temporary.buff;
        this->parent = temporary.parent;
        this->childs = temporary.childs;
        this->members = temporary.members;
        this->unresolved_calls = temporary.unresolved_calls;
        this->unresolved_returns = temporary.unresolved_returns;
    }
    FViewTable& operator=(const FViewTable& rhs);
    ~FViewTable() {
        // delete this->buff;
    };

    void setBuff(ProgramBuff* global_buff) {
        this->buff = global_buff;
    }

    int insertEntry(size_t id, char* name); //return 1 if error
    
    int insertEntry(char* name); //return 1 if error

    int insertEntry(char* name, int64_t offset); //return 1 if error

    int insertEntry(size_t id, char* name, int64_t offset); //return 1 if error

    Entry* findEntry(char* name);

    void update_size();

    void changeCurSizeDelta(int diff) { current_size += diff; };

    void process_return_jmps(int return_begin);

    void generate_all(Node* root_node);

    void generate_functions();

    //TODO: doc
    void generate_func_params(Node* node);

    //TODO: doc
    void check_func_params(FViewTable& functions_table);

    //TODO: doc
    void generate_func_body(Node* node);

    void update_code_size();

    byte_code_instance* get_func_body_begin_seq_bytes();

    byte_code_instance* get_func_body_end_seq_bytes();

    char* get_ret_seq(VarConst lhs, VarConst rhs);

    byte_code_instance* get_ret_seq_bytes();

    char* get_call_ret_seq(VarConst lhs, VarConst rhs);

    byte_code_instance* get_call_ret_seq_bytes();

    char* get_call_seq(VarConst lhs, VarConst rhs);

    byte_code_instance* get_call_seq_bytes();

    char* get_while_cond_seq(VarConst lhs, VarConst rhs);

    byte_code_instance* get_while_cond_seq_bytes();

    char* get_if_cond_seq(VarConst lhs, VarConst rhs);

    byte_code_instance* get_if_cond_seq_bytes();

    char* get_else_while_seq(VarConst lhs, VarConst rhs);
    
    byte_code_instance* get_else_while_seq_bytes();


    char* get_endl_seq(VarConst lhs, VarConst rhs);

    byte_code_instance* get_endl_seq_bytes();
    
    char* get_local_var_seq(int imm_const);

    byte_code_instance* get_local_var_seq_bytes(int offset);

    char* get_imm_const_seq(int imm_const);

    byte_code_instance* get_imm_const_seq_bytes(int imm_const);

    char* get_int_to_bool_seq(VarConst lhs, VarConst rhs);

    byte_code_instance* get_int_to_bool_seq_bytes();

    char* get_add_seq(VarConst lhs, VarConst rhs);

    byte_code_instance* get_add_seq_bytes();

    char* get_sub_seq(VarConst lhs, VarConst rhs);

    byte_code_instance* get_sub_seq_bytes();

    char* get_mul_seq(VarConst lhs, VarConst rhs);

    byte_code_instance* get_mul_seq_bytes();

    char* get_div_seq(VarConst lhs, VarConst rhs);

    byte_code_instance* get_div_seq_bytes();

    char* get_mod_seq(VarConst lhs, VarConst rhs);

    byte_code_instance* get_mod_seq_bytes();

    char* get_and_seq(VarConst lhs, VarConst rhs);

    byte_code_instance* get_and_seq_bytes();

    char* get_or_seq(VarConst lhs, VarConst rhs);

    byte_code_instance* get_or_seq_bytes();

    char* get_xor_seq(VarConst lhs, VarConst rhs);

    byte_code_instance* get_xor_seq_bytes();

    char* get_shl_seq(VarConst lhs, VarConst rhs);

    byte_code_instance* get_shl_seq_bytes();

    char* get_shr_seq(VarConst lhs, VarConst rhs);

    byte_code_instance* get_shr_seq_bytes();

    char* get_set_seq(VarConst lhs);

    byte_code_instance* get_set_seq_bytes(int offset);

    char* get_eq_seq(VarConst lhs, VarConst rhs);

    byte_code_instance* get_equal_seq_bytes();

    char* get_non_eq_seq(VarConst lhs, VarConst rhs);

    byte_code_instance* get_non_eq_seq_bytes();

    char* get_less_seq(VarConst lhs, VarConst rhs);

    byte_code_instance* get_less_seq_bytes();

    char* get_grtr_seq(VarConst lhs, VarConst rhs);

    byte_code_instance* get_grtr_seq_bytes();

    char* get_less_eq_seq(VarConst lhs, VarConst rhs);

    byte_code_instance* get_less_eq_seq_bytes();

    char* get_grtr_eq_seq(VarConst lhs, VarConst rhs);

    byte_code_instance* get_grtr_eq_seq_bytes();

    char* get_not_seq(VarConst oper);

    byte_code_instance* get_not_seq_bytes();

    char* get_inc_seq(VarConst oper);

    //TODO:
    unsigned char* get_inc_seq_bytes(VarConst lhs, VarConst rhs);

    char* get_dec_seq(VarConst oper);

    //TODO:
    unsigned char* get_dec_seq_bytes(VarConst lhs, VarConst rhs);

    void generate_code(FILE* file);

};








#endif 