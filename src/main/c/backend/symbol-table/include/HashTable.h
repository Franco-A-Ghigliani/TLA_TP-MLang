#ifndef HASH_TABLE_H
#define HASH_TABLE_H

typedef struct HashTable *HashTableADT;

HashTableADT h_init();
void h_put(HashTableADT table, char* key, SymbolTableItem *value);
SymbolTableItem* h_get(HashTableADT table, const char* key);
void h_destroy(HashTableADT table);
#endif