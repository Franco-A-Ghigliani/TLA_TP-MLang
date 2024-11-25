#ifndef HASHMAP_H
#define HASHMAP_H

typedef struct HashMap HashMap;

HashMap *createHashMap();
void freeHashMap(HashMap *map);
void insert(HashMap *map, const char *key, int value);
int get(HashMap *map, const char *key, int *found);

#endif //HASHMAP_H
