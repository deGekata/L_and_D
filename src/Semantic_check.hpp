#ifndef SEMANTIC_CHECK
#define SEMANTIC_CHECK

#include "MyVector.h"
#include "LanguageTree.hpp"
#include "CodeGenerator.hpp"
#include "LangTreeDraw.hpp"

class ProgramBuff {
public:
    void addCodeToBuff(char* listing, char* bytes, size_t bytes_len);
    ~ProgramBuff();
    void dropBuffToFile(FILE* file);
private:
    MyVector<char*> binary_program;
    MyVector<char*> progam;
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
    FViewTable() {};
    FViewTable(FViewTable* parent);
    FViewTable(FViewTable&& temporary);
    // FViewTable& operator=(const FViewTable& rhs);
    // FViewTable& operator=(FViewTable&& rhs) = default;
    // FViewTable& FViewTable::operator=(const FViewTable&) = default;
    ~FViewTable() {};
        
    int insertEntry(size_t id, char* name); //return 1 if error
    
    int insertEntry(char* name); //return 1 if error

    int insertEntry(char* name, int64_t offset); //return 1 if error

    int insertEntry(size_t id, char* name, int64_t offset); //return 1 if error

    Entry* findEntry(char* name);

    void changeCurSizeDelta(int diff) { current_size += diff; };

    void generate_functions();

    void generate_function();

    void update_code_size();

    int current_size = 0;
    
    ProgramBuff buff;
    FViewTable* parent;
    MyVector<FViewTable> childs;
    MyVector<Entry> members;
    MyVector<Entry> unresolved_calls;


};


void generate_code(FILE* file);


//TODO: doc
void generate_func_params(Node* node, FViewTable& table);

//TODO: doc
void check_func_params(FViewTable& functions_table, FViewTable& func_table);

//TODO: doc
void generate_func_body(Node* node, FViewTable& table);

// geme


#endif 