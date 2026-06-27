#ifndef AIV_LIST
#define AIV_LIST
#include <stddef.h>
#include <stdbool.h>

typedef struct aiv_list_node {
    void* data;
    struct aiv_list_node* prev;
    struct aiv_list_node* next;
} aiv_list_node_t;

typedef struct aiv_list {
    aiv_list_node_t* head;
    aiv_list_node_t* tail;
    size_t count;
} aiv_list_t;

aiv_list_t aiv_list_new();
void aiv_list_destroy(aiv_list_t* list);

void aiv_list_add(aiv_list_t* list, void* item);

bool aiv_list_is_empty(aiv_list_t* list);

#endif //AIV_LIST