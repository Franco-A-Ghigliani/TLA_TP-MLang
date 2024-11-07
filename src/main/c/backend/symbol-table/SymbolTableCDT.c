#ifndef SYMBOL_TABLE_CDT
#define SYMBOL_TABLE_CDT
#include "SymbolTableADT.h"
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
    SymbolTableItem *first;
    SymbolTableItem *current;
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
    //ESPERAR A CAMBIARLO A UN HASH
    //RECURSIVO EN LAS TABLAS INTERNAS
}

SymbolTableItem getNextItem(const SymbolTableADT table){
    SymbolTableItem *first = table->first;
    SymbolTableItem ret;
    ret.id = first->id;
    ret.next = NULL;
    ret.type = first->type;
    ret.value = first->value;
    return ret;
}

void addItem(SymbolTableADT table, SymbolTableItem* item){
    //ESPERAR A CAMBIARLO A UN HASH
}

SymbolTableItem* getItemByID(const SymbolTableADT table, const char* id){
    //ESPERAR A CAMBIARLO A UN HASH
    //RECURSIVO EN LAS TABLAS ACTIVAS
}

#endif