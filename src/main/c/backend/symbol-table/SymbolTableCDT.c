#include "SymbolTableADT.h"
#include "HashTable.h"
#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"

typedef struct SymbolTable SymbolTable;
typedef struct SymbolTableItem SymbolTableItem;
typedef union SymbolTableValue SymbolTableValue;

typedef struct SymbolTableManager
{
    SymbolTable *activeTable;
    SymbolTable *simulationTable;
    SymbolTable *mainTable;
} SymbolTableManager;


typedef struct SymbolTable
{
    HashTableADT table;
    SymbolTable *lastActiveTable;
} SymbolTable;

void exitCurrentScope(SymbolTableManagerADT manager){
    SymbolTable* next = manager->activeTable->lastActiveTable;
    manager->activeTable->lastActiveTable = NULL;
    manager->activeTable = next;
}

SymbolTableADT getActiveScope(const SymbolTableManagerADT manager){
    return manager->activeTable;
}

void setNewActiveScope(SymbolTableManagerADT manager, SymbolTableADT newTable){
    newTable->lastActiveTable = manager->activeTable;
    manager->activeTable = newTable;
}

SymbolTableADT getNextScope(const SymbolTableADT activeScope){
    return activeScope->lastActiveTable;
}

void resetScope(SymbolTableManagerADT manager){
    manager->activeTable = manager->mainTable;
}

SymbolTableManagerADT createSymbolTableManager(){
    SymbolTableADT mainTable = createSymbolTable();
    SymbolTableADT simulationTable = createSymbolTable();
    SymbolTableManager* manager = malloc(sizeof(SymbolTableManager));
    if (manager != NULL) {
        manager->mainTable = mainTable;
        manager->simulationTable = simulationTable;
        manager->activeTable = mainTable;
    }
    return manager;
}

void destroySymbolTableManager(SymbolTableManagerADT manager){
    if (manager != NULL) {
        if (manager->simulationTable != NULL) {
            destroySymbolTable(manager->simulationTable);
            manager->simulationTable = NULL; // Prevent accidental reuse
        }
        if (manager->mainTable != NULL) {
            destroySymbolTable(manager->mainTable);
            manager->mainTable = NULL;
        }
        free(manager);
    }
}

SymbolTableADT createSymbolTable(){
    SymbolTableADT table = calloc(1, sizeof(SymbolTable));
    if(table != NULL){
        table->table = h_init();
    }
    return table;
}

void destroySymbolTable(SymbolTableADT table){
    if(table != NULL){
        h_destroy(table->table);
        table->table = NULL;
        if (table->lastActiveTable != NULL) {
            SymbolTable *last = table->lastActiveTable;
            table->lastActiveTable = NULL; // Break link before recursive free
            destroySymbolTable(last);
        }
        free(table);
    }
}

void addItem(SymbolTableADT table, SymbolTableItem* item){
    if (table != NULL && item != NULL && item->id != NULL) {
        h_put(table->table, item->id, item);
    }
}

SymbolTableItem* getItemByID(const SymbolTableManagerADT manager, const char* id, boolean recursive){
    SymbolTableADT currentTable = manager->activeTable;
    while (currentTable != NULL)
    {
        SymbolTableItem *item =  h_get(currentTable->table, id);
        if(item != NULL){
            return item;
        }
        if(!recursive)
            break;
            
        currentTable = currentTable->lastActiveTable;
    }
    return NULL;
}

void freeSymbolTableItem(SymbolTableItem* item){
    switch (item->type)
    {
        case STRING:
            if(item->value.stringValue != NULL){
                free(item->value.stringValue);
                item->value.stringValue = NULL;
            }
            break;
        case SIM_TEMPLATE:
            if(item->value.simulationTemplate.internalSymbolTable != NULL){
                destroySymbolTable(item->value.simulationTemplate.internalSymbolTable);
                item->value.simulationTemplate.internalSymbolTable = NULL;
            }
            break;
        default:
            break;
    }
    free(item);
}
