#ifndef LINKED_LIST_H
#define LINKED_LIST_H

void* new_linked_list();
int size(void* list);
void insertFront (void *list, void *data);
void insertBack (void *list, void *data);
void* removeFront(void *list);
void* removeBack(void *list);
void* ll_remove(void *list, int index);
void insert (void *list,void* data, int index);
void* set(void *list, void* data, int index);
void* get(void *list, int index);

#endif
