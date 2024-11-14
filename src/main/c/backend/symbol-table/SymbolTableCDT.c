#include <SymbolTableADT.h>
#include <HashTable.h>
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

SymbolTableManagerADT createSymbolTableManager(SymbolTableADT mainTable, SymbolTableADT simulationTable){
    SymbolTableManager* manager = malloc(sizeof(SymbolTableManager));
    manager->mainTable = mainTable;
    manager->simulationTable = simulationTable;
    manager->activeTable = mainTable;
    return manager;
}

void destroySymbolTableManager(SymbolTableManagerADT manager){
    destroySymbolTable(manager->simulationTable);
    destroySymbolTable(manager->mainTable);
    free(manager);
}

SymbolTableADT createSymbolTable(){
    return malloc(sizeof(SymbolTable));
}

void destroySymbolTable(SymbolTableADT table){
    if(table != NULL){
        h_destroy(table->table);
        destroySymbolTable(table->lastActiveTable);
        free(table);
    }
}

void addItem(SymbolTableADT table, SymbolTableItem* item){
    h_put(table->table, item->id, item);
}

SymbolTableItem* getItemByID(const SymbolTableADT table, const char* id){
    return h_get(table->table, id);
}

void freeSymbolTableItem(SymbolTableItem* item){
    switch (item->type)
    {
        case STRING:
            free(item->value.stringValue);
            break;
        case NODE_TEMPLATE:
            free(item->value.nodeTemplateValue.commonParams.label);
            break;
        case SIM_TEMPLATE:
            destroySymbolTable(item->value.simulationTemplate.internalSymbolTable);
            break;
        default:
            break;
    }
}
