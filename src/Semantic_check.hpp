#ifndef SEMANTIC_CHECK
#define SEMANTIC_CHECK

#include "MyVector.h"
#include "LanguageTree.hpp"

class Entry {
    size_t id = 0;
    char* name = NULL;
    int offset = 0;
    Entry() = default;
    Entry(size_t _id, char* _name) : id(_id), name(_name) {};
    Entry(size_t _id, char* _name, int _offset) : id(_id), name(_name), offset(_offset) {};
    ~Entry() {};
};

class FViewTable {
public:
    FViewTable();
    FViewTable(FViewTable* parent);
    ~FViewTable();
    FViewTable* parent;
    MyVector<FViewTable> childs;
    MyVector<Entry> variables;
    int insertEntry(size_t id, char* name); //return 1 if error
    int insertEntry(size_t id, char* name, int offset); //return 1 if error
    Entry* findEntry(char* name);
    void changeCurSizeDelta(int diff) { current_size += diff; };
    int current_size = 0;

};


#endif 