#ifndef LINKED_LIST_H
#define LINKED_LIST_H

void* new_linked_list();
int linked_list_size(void* list);
void linked_list_insert_Front (void *list, void *data);
void linked_list_insert_Back (void *list, void *data);
void* linked_list_remove_Front(void *list);
void* linked_list_get_front(void *list);
void* linked_list_get_back(void *list);
void* linked_list_remove_back(void *list);
void* linked_list_remove(void *list, int index);
void linked_list_insert (void *list,void* data, int index);
void* linked_list_set(void *list, void* data, int index);
void* linked_list_get(void *list, int index);

#endif
