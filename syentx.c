#include <stdio.h>
#include <stdlib.h>

/* Data structures for storing the tokens and the parse tree */
typedef struct Token {
  int type;
  int value;
} Token;

typedef struct Node {
  int symbol;
  struct Node* children[10];
  int child_count;
} Node;

/* Function prototypes */
Node* Parser(Token* tokens);
void print_parse_tree(Node* root);

int main(int argc, char** argv) {
  /* Define the input tokens */
  Token tokens[] = {
    {0, 5},
    {1, 0},
    {0, 6},
    {2, 0},
    {0, 7},
    {3, 0}
  };
  int token_count = sizeof(tokens) / sizeof(Token);

  /* Parse the tokens */
  Node* root = Parser(tokens);

  /* Print the parse tree */
  print_parse_tree(root);

  return 0;
}

Node* Parser(Token* tokens) {
  /* Create an array of node pointers to store the parse tree */
  int token_count = sizeof(tokens) / sizeof(Token);
  Node* nodes[token_count];
  for (int i = 0; i < token_count; i++) {
    nodes[i] = malloc(sizeof(Node));
    nodes[i]->symbol = tokens[i].type;
    nodes[i]->child_count = 0;
  }

  /* Initialize the parse table with -1 (meaning no rule has been applied) */
  int parse_table[token_count][token_count];
  for (int i = 0; i < token_count; i++) {
    for (int j = 0; j < token_count; j++) {
      parse_table[i][j] = -1;
    }
  }

  /* Define the grammar for the language being parsed */
  int grammar[][10] = {
    {0, 1, 2},
    {0, 3},
    {1, 4, 5},
    {2, 4},
    {3, 4}
  };
  int grammar_size = sizeof(grammar) / sizeof(int[10]);

  /* Fill the parse table using a bottom-up approach */
  for (int span = 1; span <= token_count; span++) {
    for (int start = 0; start + span <= token_count; start++) {
      int end = start + span;
      for (int mid = start + 1; mid < end; mid++) {
        /* Check if combining nodes[start][mid] and nodes[mid][end] with a
           grammar rule produces a non-terminal symbol that appears on the
           right-hand side of another rule */
        int lhs = nodes[start][mid]->symbol;
        int rhs = nodes[mid][end]->symbol;
        for (int i = 0; i < grammar_size; i++) {
          if (grammar[i][0] == lhs && grammar[i][1] == rhs) {
            /* If the rule is applicable, update the parse table and create a
               new node for the non-terminal symbol */
            int symbol = grammar[i][2];
            parse_table[start][end] = symbol;
            nodes[start][end]->symbol = symbol;
            nodes[start][end]->children[0] = nodes[start][mid];
            nodes[start][end]->children[1] = nodes[mid][end];
            nodes[start][end]->child_count = 2;
            break;
          }
        }
      }
    }
  }

  /* Return the root node of the parse tree */
  return nodes[0][token_count - 1];
}

void print_parse_tree(Node* root) {
void print_parse_tree(Node* root) {
  if (root == NULL) {
    return;
  }

  if (root->child_count == 0) {
    /* If the node is a leaf node, print its symbol */
    printf("%d ", root->symbol);
  } else {
    /* If the node has children, print the symbol and recursively call
       print_parse_tree() on each child */
    printf("%d ", root->symbol);
    for (int i = 0; i < root->child_count; i++) {
      print_parse_tree(root->children[i]);
    }
  }
}
}

