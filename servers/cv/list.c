#include "list.h"

void list_init(List* l, size_t data_size) {
    l->head = calloc(1, sizeof(ListNode));
    l->tail = calloc(1, sizeof(ListNode));
    l->data_size = data_size;

    l->head->next = l->tail;
    l->tail->prev = l->head;
}

void list_destroy(List* l) {
    ListNode* node = l->head->next;

    while (node != l->tail) {
        ListNode* next = node->next;
        free(node->data);
        free(node);
        node = next;
    }
    free(l->head);
    free(l->tail);
}

bool list_empty(const List* l) { 
    return l->head->next == l->tail;
}

ListNode* list_first(const List* l) {
    if (list_empty(l))
        return NULL;
    return l->head->next;
}

ListNode* list_last(const List* l) {
    if (list_empty(l))
        return NULL;
    return l->tail->prev;
}

void list_insert(List* l, ListNode* prev, ListNode* next, const void* element) {
    ListNode* node = calloc(1, sizeof(ListNode));
    node->data = calloc(1, l->data_size);
    memcpy(node->data, element, l->data_size);

    node->prev = prev;
    node->next = next;

    prev->next = node;
    next->prev = node;
}

void list_remove(ListNode* node) {
    ListNode* prev = node->prev;
    ListNode* next = node->next;
    free(node->data);
    free(node);

    prev->next = next;
    next->prev = prev;
}

/* finds *first* (starting from head) element matching given criteria */
ListNode* list_find(const List* l, const void* element,
                    bool (*equals)(const void* a, const void* b))
{
    if (list_empty(l))
        return NULL;
    ListNode* node = list_first(l);

    while (node != l->tail) {
        if (equals(node->data, element))
            return node;
        node = node->next;
    }
    return NULL;
}

void list_push_back(List* l, const void* element) {
    list_insert(l, l->tail->prev, l->tail, element);
}

void list_push_front(List* l, const void* element) {
    list_insert(l, l->head, l->head->next, element);
}

