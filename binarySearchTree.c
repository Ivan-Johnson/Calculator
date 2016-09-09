#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "binarySearchTree.h"

binary_search_tree *new_binary_search_tree() {
  binary_search_tree *tree = malloc(sizeof(binary_search_tree));
  assert(tree != NULL);
  tree->tree = new_binary_tree(NULL, NULL, NULL);
  return tree;
}

binary_tree* getClosest(binary_tree *tree, void *data, int (*compare)(void*, void*)) {
  //TODO OPTIMIZE; DON'T RECURSE
  assert(tree != NULL && tree->data != NULL);
  int relativePosition = compare(data, tree->data);
  if (relativePosition < 0) {
    if (tree->left == NULL)
      return tree;
    else
      return getClosest(tree->left, data, compare);
  } else if (relativePosition > 0) {
    if (tree->right == NULL)
      return tree;
    else
      return getClosest(tree->right, data, compare);
  } else {
    assert(relativePosition == 0);
    return tree;
  }
}

void BST_insert(binary_search_tree *tree, void *data, int (*compare)(void*, void*)) {
  assert(data != NULL && tree != NULL && compare != NULL);
  if (tree->tree->data == NULL) {
    tree->tree->data = data;
    return;
  }
  binary_tree *location = getClosest(tree->tree, data, compare);
  int relativePosition = compare(data, location->data);
  if (relativePosition < 0) {
    assert(location->left == NULL);
    location->left = new_binary_tree(NULL, NULL, data);
    return;
  }
  if (relativePosition > 0) {
    assert(location->right == NULL);
    location->right = new_binary_tree(NULL, NULL, data);
    return;
  }
  assert(relativePosition == 0);
  printf("ERROR WHEN INSERTING DATA TO BINARY SEARCH TREE: NODE WITH THAT DATA ALREADY EXISTS\n");
  exit(EXIT_FAILURE);
}

void *BST_get(binary_search_tree *tree, void *data, int (*compare)(void*, void*)) {
  assert(data != NULL && tree != NULL && compare != NULL);
  binary_tree *location = getClosest(tree->tree, data, compare);
  if (location == NULL || compare(data, location->data) != 0)
    return NULL;
  return location->data;
}
