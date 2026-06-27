#ifndef AIV_DICT
#define AIV_DICT

#include <stddef.h>

typedef struct aiv_dict_node {
    void* key;
    size_t key_size;
    void* value;
    struct aiv_dict_node* next;
} aiv_dict_node_t;

typedef struct aiv_dict {
    size_t hashmap_size;
    void** hashmap;
    size_t (*hash_func)(void*, size_t);
} aiv_dict_t;

aiv_dict_t aiv_dict_new();

aiv_dict_t aiv_dict_new_with_params(size_t hashmap_size, size_t (*hash_func)(void *key, size_t keylen));

void aiv_dict_put(aiv_dict_t*, void* key, size_t key_size, void* value);

#endif //AIV_DICT