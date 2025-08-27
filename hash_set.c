#include "hash_set.h"

// Simple hash function using djb2 algorithm
static unsigned int hash_function_djb2(const char *key, size_t size) {
    unsigned int hash = 5381;
    int c;
    
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    
    return hash % size;
}

// Create a new hash set
// return a pointer to the created hash set, or NULL on failure
hash_set* hash_set_create(void) {
    hash_set *set = malloc(sizeof(hash_set));
    if (!set) return NULL;
    
    // create buckets
    set->size = HASH_SET_SIZE;
    set->buckets = calloc(set->size, sizeof(hash_node*));
    
    // fail check
    if (!set->buckets) {
        free(set);
        return NULL;
    }
    
    return set;
}

// Destroy hash set and free memory
void hash_set_destroy(hash_set *set) {
    if (!set) return;
    
    for (size_t i = 0; i < set->size; i++) {
        hash_node *current = set->buckets[i];
        // destroy the linked list node by node
        while (current) {
            hash_node *temp = current;
            current = current->next;
            free(temp->key);
            free(temp);
        }
    }
    
    free(set->buckets);
    free(set);
}

// Insert a string into the hash set
// return: RET_C: successfully inserted
//         0: already exists 
//        -1: error 
FunctionStatus  hash_set_insert(hash_set *set, const char *key) {
    if (!set || !key) return NULL_INPUT_POINTER;
    
    // compute hash value
    unsigned int hash_value = hash_function_djb2(key, set->size);

    // locate the bucket
    hash_node *current = set->buckets[hash_value];

    // check for appearance
    while (current) {
        if (strcmp(current->key, key) == 0) {
            return FALSE_STATUS; // already has the key, do not insert
        }
        current = current->next;
    }

    // Create new node
    hash_node *new_node = malloc(sizeof(hash_node));
    if (!new_node) return MEMORY_ALLOCATION_ERR;
    
    new_node->key = malloc(strlen(key) + 1);
    if (!new_node->key) {
        free(new_node);
        return MEMORY_ALLOCATION_ERR;
    }

    strcpy(new_node->key, key);
    
    // Insert at the beginning of the chain
    new_node->next = set->buckets[hash_value];
    set->buckets[hash_value] = new_node;
    
    return TRUE_STATUS;
}

// Check if a string exists in the hash set
// return: 1: exists
//         0: does not exist 
FunctionStatus  hash_set_contains(hash_set *set, const char *key) {
    if (!set || !key) return NULL_INPUT_POINTER;

    unsigned int index = hash_function_djb2(key, set->size);
    hash_node *current = set->buckets[index];

    // Traverse the linked list at this bucket
    while (current) {
        if (strcmp(current->key, key) == 0) {
            return TRUE_STATUS;
        }
        current = current->next;
    }
    
    return FALSE_STATUS;
}

// Print all elements in the hash set
void hash_set_print(FILE *ofile, hash_set *set) {
    if (!set) return;
    
    if (ofile == NULL) {
        printf("Hash Set Contents (%zu elements):\n", set->size);
        for (size_t i = 0; i < set->size; i++) {
            hash_node *current = set->buckets[i];
            while (current) {
                printf("  \"%s\"\n", current->key);
                current = current->next;
            }
        }
    } else {
        // fprintf(ofile, "Hash Set Contents (%zu elements):\n", set->size);
        for (size_t i = 0; i < set->size; i++) {
            hash_node *current = set->buckets[i];
            while (current) {
                fprintf(ofile, "%s\n", current->key);
                current = current->next;
            }
        }
    }
}

// Get the size of the hash set
size_t hash_set_get_size(hash_set *set) {
    return set ? set->size : 0;
}

// Convert hash set to array of strings
char** hash_set_to_array(hash_set *set, size_t *count) {
    if (!set || !count) return NULL;
    
    *count = set->size;
    if (set->size == 0) return NULL;
    
    char **array = malloc(set->size * sizeof(char*));
    if (!array) return NULL;
    
    size_t index = 0;
    for (size_t i = 0; i < set->size; i++) {
        hash_node *current = set->buckets[i];
        while (current) {
            array[index] = malloc(strlen(current->key) + 1);
            if (array[index]) {
                strcpy(array[index], current->key);
                index++;
            }
            current = current->next;
        }
    }
    
    return array;
}

