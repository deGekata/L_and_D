#include "LangTreeDraw.hpp"

const size_t Max_out_file_len = 150;
const size_t Max_cmd_len  = 149;


const char* Img_dump_dir = "./dump/";

const char* Next_link_color = "blue";
const char* Prev_link_color = "red";
const char* Free_link_color = "orange";

void draw_tree(Node* node) {
    static int dumpNumber = 0;
    printf("graphing %d\n", dumpNumber);

    char filename[Max_out_file_len];
    sprintf(filename, "%sLIST_DMP_№%d.dot", Img_dump_dir, dumpNumber);
    
    FILE* file = fopen(filename, "w");
    assert(file && "cant open file");

    fprintf(file,   "digraph G{\n");
    fprintf(file,   "   nodesep=0.1;\n");
    fprintf(file,   "   ratio=1;\n");
    
    draw_node(node, file);

    fprintf(file, "}");
    fclose(file);

    char command[Max_cmd_len] = {};

    sprintf(command, "dot %sLIST_DMP_№%d.dot -T png -o %sLIST_DMP_№%d.png", Img_dump_dir, dumpNumber, Img_dump_dir, dumpNumber);
    // sprintf(command, "dot %sLIST_DMP_№%d.dot -Gsize=20,20! -T png -o %sLIST_DMP_№%d.png", Img_dump_dir, dumpNumber, Img_dump_dir, dumpNumber);
    system(command);
    ++dumpNumber;
}

void draw_node(Node* node, FILE* output) {
    assert(node && "node must not be NULL");

    fprintf(output, "   L%lu[label=", node);
    switch (node->type) {
    case NodeType::IDENTIFIER :
        draw_identifier(node, output);
        break;
    
    case NodeType::OPERATOR :
        draw_operator(node, output);
        break;
    
    case NodeType::NUMBER :
        draw_number(node, output);
        break;
    
    default:
        //TODO: raise error
        printf("raise error unknown type");
        assert(0);
        break;
    }
    
    fprintf(output, " style=filled];\n");

    if (node->left != NULL)  { 
        draw_node(node->left,  output);
        fprintf(output, "    L%lu->L%lu[color=\"black\"];\n", node, node->left);
    }
    if (node->right != NULL) { 
        draw_node(node->right,  output);
        fprintf(output, "    L%lu->L%lu[color=\"black\"];\n", node, node->right);
    }

}

void draw_number(Node* node, FILE* output) {
    assert(node && "node must not be NULL");
    assert(output && "output must not be NULL");
    assert(node->type == NodeType::NUMBER && "node->type must be NodeType::NUMBER");

    fprintf(output, "\"%d\", shape=\"circle\", color=\"grey\", thickness=4, fillcolor=\"", node->data.num);
    fprintf(output, "darkolivegreen2\"");
    return;
}

void draw_operator(Node* node, FILE* output) {
    assert(node && "node must not be NULL");
    assert(output && "output must not be NULL");
    assert(node->type == NodeType::OPERATOR && "node->type must be NodeType::OPERATOR");

    fprintf(output, "\""); 

    switch (node->data.opr)
    {
    case Operator::ADD :
        fprintf(output, "%s", "+");
        break;
    
    case Operator::SUB :
        
        fprintf(output, "%s", "-");
        break;
    
    case Operator::MUL :
        
        fprintf(output, "%s", "*");
        break;
    
    case Operator::DIV :
        
        fprintf(output, "%s", "/");
        break;
    
    case Operator::SHL :
        
        fprintf(output, "%s", "<<");
        break;
    
    case Operator::SHR :
        
        fprintf(output, "%s", ">>");
        break;
    
    case Operator::MOD :
        
        fprintf(output, "%s", "%");
        break;
    
    case Operator::OR :
        
        fprintf(output, "%s", "||");
        break;
    
    case Operator::XOR :
        
        fprintf(output, "%s", "^");
        break;
    
    case Operator::AND :
        
        fprintf(output, "%s", "&&");
        break;
    
    case Operator::SET :
        
        fprintf(output, "%s", "=");
        break;
    
    case Operator::INC :
        
        fprintf(output, "%s", "++");
        break;
    
    case Operator::DEC :
        
        fprintf(output, "%s", "--");
        break;
    
    case Operator::GRTR :
        
        fprintf(output, "%s", ">");
        break;
    
    case Operator::GRTR_EQ :
        
        fprintf(output, "%s", ">=");
        break;
    
    case Operator::LESS :
        
        fprintf(output, "%s", "<");
        break;
    
    case Operator::LESS_EQ :
        
        fprintf(output, "%s", "<=");
        break;
    
    case Operator::NOT :
        
        fprintf(output, "%s", "!");
        break;
    
    case Operator::EQUAL :
        
        fprintf(output, "%s", "==");
        break;
    
    case Operator::NON_EQ :
        
        fprintf(output, "%s", "!=");
        break;
    
    
    
    default:
        break;
    }

    fprintf(output, "\", shape=\"circle\", color=\"grey\", thickness=4, fillcolor=\"darkolivegreen2\"");

    return ;
}

void draw_identifier(Node* node, FILE* output) {
    assert(node && "node must not be NULL");
    assert(output && "output must not be NULL");
    assert(node->type == NodeType::IDENTIFIER && "node->type must be NodeType::IDENTIFIER");

    fprintf(output, "\"%s\", shape=\"circle\", color=\"grey\", thickness=4, fillcolor=\"", node->name);
    fprintf(output, "cornflowerblue\"");
    return;
}
