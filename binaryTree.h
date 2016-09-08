#ifndef BINARYTREE_H
#define BINARYTREE_H
#include <stdbool.h>

struct binary_tree{
  struct binary_tree *left, *right;
  void *data;
};

typedef struct binary_tree binary_tree;
  
binary_tree* new_binary_tree(binary_tree *left, binary_tree *right, void *data);

bool setData (binary_tree *tree, void *data);
bool setLeft (binary_tree *tree, binary_tree *left);
bool setRight(binary_tree *tree, binary_tree *right);

void* getData(binary_tree *tree);
binary_tree* getLeft(binary_tree *tree);
binary_tree* getRight(binary_tree *tree);

void traversePreorder(binary_tree *tree, void (*function)(void *data));
void traverseInorder(binary_tree *tree, void (*function)(void *data));
void traversePostorder(binary_tree *tree, void (*function)(void *data));
#endif
