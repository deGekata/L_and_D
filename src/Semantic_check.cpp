#include "Semantic_check.hpp"
ProgramBuff::~ProgramBuff() {
    for (size_t it = 0; it < progam.size(); ++it)
        free(progam[it]);
    progam.~MyVector();
}