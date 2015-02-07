#ifndef LIST_H
#define LIST_H
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

typedef struct ListNode {
    void* data;
    struct ListNode* next;
    struct ListNode* prev;
} ListNode;

typedef struct List {
    ListNode* head;
    ListNode* tail;
    size_t data_size;
} List;

void list_init(List* l, size_t data_size);
void list_destroy(List* l);
bool list_empty(const List* l); 
ListNode* list_first(const List* l);
ListNode* list_last(const List* l);
void list_insert(List* l, ListNode* prev, ListNode* next, const void* element);
void list_remove(ListNode* node);
/* finds *first* (starting from head) element matching given criteria */
ListNode* list_find(const List* l, const void* element,
                    bool (*equals)(const void* a, const void* b));
void list_push_back(List* l, const void* element);
void list_push_front(List* l, const void* element);

#endif
