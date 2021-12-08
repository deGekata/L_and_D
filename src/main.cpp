#include "LangTree.hpp"
#include <stdio.h>
#include "Tokenizer.hpp"
#include "LanguageTree.hpp"
#include "LangTreeDraw.hpp"

int main() {
    Lexer* lexer = (Lexer*) calloc(1, sizeof(Lexer));
    FILE* file = fopen("test.txt", "r");

    init_lexer(lexer, file);

    

    Node* node = parse_muldiv(lexer);    
    printf("node is %d\n", node);
    draw_tree(node);

    // Node* node = get_node(lexer);

    // while (lexer->cur_token != NULL) {
    //     printf("%d line\n", __LINE__);
    //     pop_node(lexer);
    //     // printf("kek\n");
    //     switch (node->type) {
    //     case NodeType::NUMBER:
    //         printf("number: %d\n", node->data.num);
    //         break;
        
    //     case NodeType::OPERATOR :
    //         printf("Operator: %d\n", node->data.opr);
    //         break;
        

    //     case NodeType::IDENTIFIER:
    //         printf("Identifier: %s\n", node->name);
    //         break;
                
    //     default:
    //         printf("default\n");
    //         break;
    //     }

    //     node = get_node(lexer);
    //     printf("token get = %d\n", node);
    //     printf("lexer cur = %d\n", lexer->cur_token);
        
    // }
    

    // printf("%d", get_node(lexer)->data.num);
    return 0;
}