#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H
#include "binaryTree.h"

struct binary_search_tree {
    binary_tree *tree;
};
typedef struct binary_search_tree binary_search_tree;

extern binary_search_tree *new_binary_search_tree();

extern void BST_insert(binary_search_tree *tree, void *data, int (*compare)(void*, void*));
extern void *BST_get(binary_search_tree *tree, void *data, int (*compare)(void*, void*));

#endif
