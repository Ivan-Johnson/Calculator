#ifndef LINKED_LIST_H
#define LINKED_LIST_H

extern void* new_linked_list();
extern int linked_list_size(void* list);
extern void linked_list_insert_Front(void *list, void *data);
extern void linked_list_insert_Back(void *list, void *data);
extern void* linked_list_remove_Front(void *list);
extern void* linked_list_get_front(void *list);
extern void* linked_list_get_back(void *list);
extern void* linked_list_remove_back(void *list);
extern void* linked_list_remove(void *list, int index);
extern void linked_list_insert(void *list, void* data, int index);
extern void* linked_list_set(void *list, void* data, int index);
extern void* linked_list_get(void *list, int index);

#endif
