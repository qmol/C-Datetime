#ifndef __hash_set_h__
#define __hash_set_h__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "func_status.h"

#define HASH_SET_SIZE           10000000
#define LOAD_FACTOR_THRESHOLD   0.75
#define NUM_COLID_KEY           100

// Hash set node structure
typedef struct hash_node_struct {
    char * key;
    struct hash_node_struct *next;
} hash_node;

// Hash set structure
typedef struct hash_set_struct {
    hash_node **buckets;
    size_t size;
    // size_t size;
} hash_set;

// Function declarations
hash_set* hash_set_create(void);
void hash_set_destroy(hash_set *hset);
FunctionStatus  hash_set_insert(hash_set *hset, const char *key);
FunctionStatus  hash_set_contains(hash_set *hset, const char *key);
void hash_set_print(FILE *ofile, hash_set *hset);
size_t hash_set_get_size(hash_set *hset);
char** hash_set_to_array(hash_set *hset, size_t *count);

#endif // __hash_set_h__
