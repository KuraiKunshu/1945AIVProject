#include "aiv_dict.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Bernstein classic hash
static size_t djb33x_hash(void *key, size_t keylen) {
    register int hash = 5381;
    int i;

    unsigned char *key_as_num = (unsigned char *)key;

    for (i = 0; i < keylen; i++) {
        hash = ((hash << 5) + hash) ^ key_as_num[i];
    }
    return hash;
}

aiv_dict_t aiv_dict_new() {
    return aiv_dict_new_with_params(10, djb33x_hash);
}

aiv_dict_t aiv_dict_new_with_params(size_t hashmap_size, size_t (*hash_func)(void *key, size_t keylen)) {
    aiv_dict_t dict;
    dict.hashmap_size = hashmap_size;
    dict.hash_func = hash_func;
    dict.hashmap = (void**)calloc(hashmap_size, sizeof(void*));
    return dict;
}

void aiv_dict_put(aiv_dict_t* dict, void* key, size_t key_size, void* value) {
    size_t hash =  dict->hash_func(key, key_size);
    size_t hash_index = hash % dict->hashmap_size;

    // Scenario 1: hash(key) not present
    if (!dict->hashmap[hash_index]) {
        aiv_dict_node_t* node = malloc(sizeof(aiv_dict_node_t));
        //node->key = key;
        node->key = malloc(sizeof(key_size));
        memcpy(node->key, key, key_size);
        node->key_size = key_size;
        node->value = value;
        node->next = NULL;
        dict->hashmap[hash_index] = node;
    } else {  // Scenario 2: hash(key) present

        // key already exists (update value)
        aiv_dict_node_t* node = dict->hashmap[hash_index];
        aiv_dict_node_t* last = NULL;
        while(node) {
            last = node;

            //if (node->key == key) {
            if (node->key_size == key_size && !memcmp(node->key, key, key_size)) {
                node->value = value;
                return;
            }
            node = node->next;
        }

        // key not exists (to be added)
        aiv_dict_node_t* new_node = malloc(sizeof(aiv_dict_node_t));
        //new_node->key = key;
        new_node->key = malloc(sizeof(key_size));
        memcpy(new_node->key, key, key_size);
        new_node->key_size = key_size;
        new_node->value = value;
        new_node->next = NULL;

        last->next = new_node;
    }

}