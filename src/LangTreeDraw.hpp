#ifndef LANG_TREE_DRAW 
#define LANG_TREE_DRAW

#include <stdio.h>
#include <assert.h>
#include "stdlib.h"
#include "LangTree.hpp"

void draw_tree(Node* node);

void draw_node(Node* node, FILE* output);

void draw_number(Node* node, FILE* output);

void draw_operator(Node* node, FILE* output);

void draw_identifier(Node* node, FILE* output);

#endif