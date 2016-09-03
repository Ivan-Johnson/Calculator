#include "linked_list.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
//################################################################
//#############################PRIVTE#############################
//################################################################

struct linked_list_node {
  void* data;
  struct linked_list_node* forward;
  struct linked_list_node* backward;
};
typedef struct linked_list_node linked_list_node;

struct linked_list {
  int size;
  linked_list_node* front;
  linked_list_node* back;
};
typedef struct linked_list linked_list;

linked_list_node* new_linked_list_node(void* data) {
  linked_list_node *node = malloc(sizeof(linked_list_node));
  assert(node != NULL);
  node->data = data;
  node->forward = NULL;
  node->backward = NULL;

  return node;
}

linked_list_node* get_node(linked_list *list, int index) {
  if (index == list->size)
    return NULL;
  assert(list != NULL);
  assert(index >= 0 && index < list->size);
  assert(list->front != NULL && list->back != NULL);
  linked_list_node *node;
  if (index < list->size / 2) {
    node = list->front;
    for (int i = 0; i < index; i++) {
      assert(node != NULL);
      node = node->forward;
    }
  } else {
    node = list->back;
    for (int i = list->size - 1; i > index; i--) {
      assert(node != NULL);
      node = node->backward;
    }
  }
  return node;
}




//################################################################
//#############################PUBLIC#############################
//################################################################

void* new_linked_list() {
  linked_list *list = malloc(sizeof(linked_list));
  assert(list != NULL);
  list->size = 0;
  list->front = NULL;
  list->back = NULL;
  return list;
}

void* linked_list_set(void *list, void* data, int index) {
  linked_list *l = (linked_list*) list;
  linked_list_node *node = get_node(l, index);
  void* old_data = node->data;
  node->data = data;
  return old_data;
}

void linked_list_insert(void* list, void* data, int index) {
  assert(list != NULL);
  linked_list *l = (linked_list*) list;
  linked_list_node *new_node = new_linked_list_node(data);
  if (l->front == NULL || l->back == NULL) {//SPECIAL CASE: FIRST NODE BEING INSERTED
    assert(l->front == NULL && l->back == NULL); //if this assertion fails, then I've messed up. Messed up bad.
    assert(index == 0);
    l->front = new_node;
    l->back = new_node;
    l->size++;
    return;
  }
  if (index == l->size) { //SPECIAL CASE: BEING INSERTED AT BACK; REQUIRES l->back TO BE UPDATED
    new_node->forward = NULL; //redundant, for the sake of clarity
    new_node->backward = l->back;
    l->back->forward = new_node; //we know that l->back != NULL because of the prior if statment
    l->back = new_node;
    l->size++;
    return;
  }
  if (index == 0) { //SPECIAL CASE: BEING INSERTED AT FRONT; REQUIRES l->front TO BE UPDATED
    new_node->forward = l->front;
    new_node->backward = NULL; //redundant, just here so that you know that I know what I'm doing
    l->front->backward = new_node;
    l->front = new_node;
    l->size++;
    return;
  }

  linked_list_node *node = get_node(l, index);

  new_node->backward = node->backward;
  new_node->forward = node;
  if (node->backward != NULL)
    node->backward->forward = new_node;
  else
    l->front = new_node;
  node->backward = new_node;
  l->size++;
}

void* linked_list_remove(void *list, int index) {
  assert(index >= 0 && index < linked_list_size(list));
  linked_list *l = (linked_list*) list;
  linked_list_node *node = get_node(l, index);
  if (node->backward != NULL)
    node->backward->forward = node->forward;
  else
    l->front = node->forward;
  if (node->forward != NULL)
    node->forward->backward = node->backward;
  else
    l->back = node->backward;
  void* removed_data = node->data;
  free(node);
  l->size--;
  return removed_data;
}

void* linked_list_get(void *list, int index) {
  linked_list *l = (linked_list*) list;
  linked_list_node *node = get_node(l, index);
  return node->data;
}

int linked_list_size(void* list) {
  return((linked_list*) list)->size;
}

void linked_list_insert_Front(void *list, void *data) {
  linked_list_insert(list, data, 0);
}

void linked_list_insert_Back(void *list, void *data) {
  linked_list_insert(list, data, linked_list_size(list));
}

void* linked_list_get_front(void *list) {
  return linked_list_get(list, 0);
}

void* linked_list_get_back(void *list) {
  return linked_list_get(list, linked_list_size(list) - 1);
}

void* linked_list_remove_Front(void *list) {
  return linked_list_remove(list, 0);
}

void* linked_list_remove_back(void *list) {
  return linked_list_remove(list, linked_list_size(list) - 1);
}
