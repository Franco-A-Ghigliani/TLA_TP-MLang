#include <HashTable.h>
#include <SymbolTableADT.h>

#define INITIAL_TABLE_SIZE 100
#define LOAD_THRESHOLD 0.75f

static void freeListTable(PairList *list, unsigned long size);

typedef struct Pair {
    char *key;
    SymbolTableItem *value;
    struct Pair *next;
} Pair;

typedef struct PairList {
    unsigned long itemCount;
    Pair *first;
} PairList;

typedef struct HashTable {
    unsigned long size;
    unsigned long slotsOccupied;
    PairList *table;
} HashTable;

static unsigned long hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c;

    return hash;
}

static PairList* rehash(HashTableADT table){
    unsigned long oldSize = table->size;

    PairList *newTable = calloc(oldSize * 2, sizeof(PairList));
    PairList *oldTable = table->table;

    table->table = newTable;
    table->size *= 2; 
    table->slotsOccupied = 0;

    for (size_t i = 0; i < oldSize; i++)
    {
        PairList list = oldTable[i];
        if(list.itemCount > 0){
            Pair *current = list.first;
            while (current != NULL)
            {
                h_put(table, current->key, current->value);
            }
        }
    }

    freeListTable(oldTable, oldSize);

    if(((float)(table->size)/(float)(table->slotsOccupied)) > LOAD_THRESHOLD){
        table = rehash(table);
    }
}

HashTableADT h_init(){
    HashTableADT table = calloc(1, sizeof(HashTable));
    table->table = calloc(INITIAL_TABLE_SIZE, sizeof(PairList));
    table->size = INITIAL_TABLE_SIZE;
    return table;
}

void h_put(HashTableADT table, const char* key, const SymbolTableItem *value){
    unsigned long hashVal = hash(key) % table->size;
    PairList list = (table->table)[hashVal];

    if(list.itemCount == 0){
        table->slotsOccupied++;
    }

    Pair *current = list.first;
    while (current != NULL)
    {
        if(strcmp(key, current->key) == 0){ //If the key already exists, assign new value
            freeSymbolTableItem(current->value);
            current->value = value;
        } else if(current->next == NULL){   //If this is the last item, add it to the end of the list
            Pair * newElem = calloc(1, sizeof(Pair));
            newElem->key = malloc(sizeof(char) * (strlen(key) + 1));
            strcpy(newElem->key, key);
            newElem->value = value;
            current->next = newElem;
            list.itemCount++;
        }
        current = current->next;
    }
    if(((float)(table->size)/(float)(table->slotsOccupied)) > LOAD_THRESHOLD){
        table = rehash(table);
    }
}

SymbolTableItem* h_get(HashTableADT table, const char* key){
    unsigned long hashVal = hash(key) % table->size;
    PairList list = (table->table)[hashVal];

    Pair *current = list.first;
    while (current != NULL)
    {
        if(strcmp(key, current->key) == 0){ //Found it
            return &(current->value);
        }
        current = current->next;
    }
    return NULL;
}

void h_destroy(HashTableADT table){
    if(table != NULL){
        if(table->table != NULL)
            freeListTable(table->table, table->size);
        free(table);
    }
}

static void freeListTable(PairList *list, unsigned long size){
    for (size_t i = 0; i < size; i++)
    {
        if(list->itemCount > 0){
            Pair *current = list->first;
            while (current != NULL)
            {
                free(current->key);
                freeSymbolTableItem(current->value);
            }
            current = current->next;
        }
    }
}