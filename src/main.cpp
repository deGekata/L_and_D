#include "LangTree.hpp"
#include <stdio.h>
#include "Tokenizer.hpp"

int main() {
    Lexer* lexer = (Lexer*) calloc(1, sizeof(Lexer));
    FILE* file = fopen("test.txt", "r");

    init_lexer(lexer, file);
    Node* node = get_node(lexer);
    
    while (lexer->cur_token != NULL) {
        pop_node(lexer);
        // printf("kek\n");
        switch (node->type) {
        case NodeType::NUMBER:
            printf("number: %d\n", node->data.num);
            break;
        
        case NodeType::OPERATOR :
            printf("Operator: %d\n", node->data.opr);
            break;
        

        case NodeType::IDENTIFIER:
            printf("Identifier: %s\n", node->name);
            break;
                
        default:
            printf("default\n");
            break;
        }

        node = get_node(lexer);
        
    }
    

    // printf("%d", get_node(lexer)->data.num);
    return 0;
}