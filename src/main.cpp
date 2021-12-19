#include "LangTree.hpp"
#include <stdio.h>
#include "Tokenizer.hpp"
#include "LanguageTree.hpp"
#include "LangTreeDraw.hpp"




int main(int argc, char* argv[]) {
    Lexer* lexer = (Lexer*) calloc(1, sizeof(Lexer));
    FILE* file = fopen("/mnt/c/Users/Yaric-PC/Documents/Git Projects/L_and_D/test.txt", "r");
    printf("%s file loc\n", argv[0]);
    init_lexer(lexer, file);
    init_operator_hashes();
    
    printf("%s \n", __FUNCTION__);

    Node* node = parse_exprs(lexer);    
    printf("node is %d\n", node);
    draw_tree(node);

    // printf(" %d result \n";

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