#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "binaryTree.h"

binary_tree* new_binary_tree(binary_tree *left, binary_tree *right, void *data){
  binary_tree *tree = malloc(sizeof(binary_tree));
  assert(tree != NULL);
  
  tree->left = left;
  tree->right = right;
  tree->data = data;
  return tree;
}

binary_tree* getLeft(binary_tree *tree){
  return tree->left;
}
binary_tree* getRight(binary_tree *tree){
  return tree->right;
}
void* getData(binary_tree *tree){
  return tree->data;
}

void traversePreorder(binary_tree *tree, void (*function)(void *data)){
  function(tree->data);

  if (tree->left != NULL){
    traversePreorder(tree->left, function);
  }

  if (tree->right != NULL){
    traversePreorder(tree->right, function);
  }
}

void traverseInorder(binary_tree *tree, void (*function)(void *data)){
  if (tree->left != NULL){
    traverseInorder(tree->left, function);
  }

  function(tree->data);
  
  if (tree->right != NULL){
    traverseInorder(tree->right, function);
  }
}

void traversePostorder(binary_tree *tree, void (*function)(void *data)){
  if (tree->left != NULL){
    traversePostorder(tree->left, function);
  }
  
  if (tree->right != NULL){
    traversePostorder(tree->right, function);
  }
  
  function(tree->data);
}

bool setData(binary_tree *tree, void *data){
  assert(tree != NULL);
  bool val = data != NULL;
  tree->data = data;
  return val;
}

bool setLeft(binary_tree *tree, binary_tree* l){
  assert(tree != NULL);
  bool val = tree->left != NULL;
  tree->left = l;
  return val;
}

bool setRight(binary_tree *tree, binary_tree* r){
  assert(tree != NULL);
  bool val = tree->right != NULL;
  tree->right = r;
  return val;
}
