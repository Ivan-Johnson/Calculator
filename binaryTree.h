#ifndef BINARYTREE_H
#define BINARYTREE_H
#include <stdbool.h>

struct binary_tree {
    struct binary_tree *left, *right;
    void *data;
};

typedef struct binary_tree binary_tree;

extern binary_tree* new_binary_tree(binary_tree *left, binary_tree *right, void *data);

extern bool setData(binary_tree *tree, void *data);
extern bool setLeft(binary_tree *tree, binary_tree *left);
extern bool setRight(binary_tree *tree, binary_tree *right);

extern void* getData(binary_tree *tree);
extern binary_tree* getLeft(binary_tree *tree);
extern binary_tree* getRight(binary_tree *tree);

extern void traversePreorder(binary_tree *tree, void (*function)(void *data));
extern void traverseInorder(binary_tree *tree, void (*function)(void *data));
extern void traversePostorder(binary_tree *tree, void (*function)(void *data));
#endif
