#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHMAP_INITIAL_CAPACITY 16
#define HASHMAP_LOAD_FACTOR 0.75

typedef struct HashNode {
    char *key;
    int value;
    struct HashNode *next;
} HashNode;

typedef struct {
    HashNode **buckets;
    size_t capacity;
    size_t size;
} HashMap;

unsigned int hashFunction(const char *key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash * 31) + (unsigned char)(*key++);
    }
    return hash;
}

HashNode *createNode(const char *key, int value) {
    HashNode *node = (HashNode *)malloc(sizeof(HashNode));
    node->key = strdup(key);
    node->value = value;
    node->next = NULL;
    return node;
}

HashMap *createHashMap() {
    HashMap *map = (HashMap *)malloc(sizeof(HashMap));
    map->capacity = HASHMAP_INITIAL_CAPACITY;
    map->size = 0;
    map->buckets = (HashNode **)calloc(map->capacity, sizeof(HashNode *));
    return map;
}

void insert(HashMap *map, const char *key, int value) {
    unsigned int hash = hashFunction(key) % map->capacity;
    HashNode *current = map->buckets[hash];

    while (current) {
        if (strcmp(current->key, key) == 0) {
            current->value = value;
            return;
        }
        current = current->next;
    }

    // Insertar un nuevo nodo
    HashNode *newNode = createNode(key, value);
    newNode->next = map->buckets[hash];
    map->buckets[hash] = newNode;
    map->size++;
}

int get(HashMap *map, const char *key, int *found) {
    unsigned int hash = hashFunction(key) % map->capacity;
    HashNode *current = map->buckets[hash];

    while (current) {
        if (strcmp(current->key, key) == 0) {
            *found = 1;
            return current->value;
        }
        current = current->next;
    }

    *found = 0;
    return 0;
}

void freeNode(HashNode *node) {
    free(node->key);
    free(node);
}

void freeHashMap(HashMap *map) {
    for (size_t i = 0; i < map->capacity; i++) {
        HashNode *current = map->buckets[i];
        while (current) {
            HashNode *temp = current;
            current = current->next;
            freeNode(temp);
        }
    }
    free(map->buckets);
    free(map);
}




