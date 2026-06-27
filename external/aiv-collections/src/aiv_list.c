#include "aiv_list.h"
#include <stdlib.h>

aiv_list_t aiv_list_new() {
    aiv_list_t list;
    list.head = NULL;
    list.tail = NULL;
    list.count = 0;
    return list;
}

void aiv_list_destroy(aiv_list_t* list) {
    aiv_list_node_t* current = list->head;
    while(current) {
        aiv_list_node_t* next = current->next;
        free(current);
        current = next;
    }
}


bool aiv_list_is_empty(aiv_list_t* list) {
    return list->count == 0;
}

void aiv_list_add(aiv_list_t* list, void* item) {
    aiv_list_node_t* node = (aiv_list_node_t*)malloc(sizeof(aiv_list_node_t));
    node->data = item;
    node->prev = NULL;
    node->next = NULL;

    if (aiv_list_is_empty(list)) {
        list->head = node;
        list->tail = node;
    } else {   //  node1 <-- node2  
        aiv_list_node_t* prev = list->tail;
        node->prev = prev;
        prev->next = node;
        list->tail = node;
    }

    list->count++;
}