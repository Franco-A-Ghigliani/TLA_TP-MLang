#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"

typedef struct SymbolTableManager* SymbolTableManagerADT;
typedef struct SymbolTable* SymbolTableADT;

typedef enum
{
    INT_VALUE,
    STRING_VALUE,
    NODE,
    NODE_TEMPLATE,
    NODE_TEMPLATE_INSTANCE,
    SIM_TEMPLATE,
    SIM_TEMPLATE_INSTANCE,
    BOTTOM
} SymbolTableValueType;


typedef struct SimulationTemplateSymbol
{
    SymbolTableADT internalSymbolTable;
} SimulationTemplateSymbol;

typedef union SymbolTableValue
{
    char *stringValue;
    int intValue;

    SimulationNode *nodeInTree;

    struct NodeInstance
    {
        SimulationNode *originalTemplateInTree;
        NodeParams *instanceOverridesinTree;
    } nodeInstance;

    SimulationTemplateSymbol simulationTemplate;
    SimulationTemplateSymbol *simulationTemplateInstanceParent;
} SymbolTableValue;

typedef struct SymbolTableItem
{
    char *id;
    SymbolTableValueType type;
    SymbolTableValue value;
} SymbolTableItem;


void exitCurrentScope(SymbolTableManagerADT manager);
SymbolTableADT getActiveScope(const SymbolTableManagerADT manager);
void setNewActiveScope(SymbolTableManagerADT manager, SymbolTableADT newTable);
void setSimulationScopeAsActive(SymbolTableManagerADT manager);
SymbolTableADT getNextScope(const SymbolTableADT activeScope);
void resetScope(SymbolTableManagerADT manager);

// Functions to create and destroy symbol table structures
SymbolTableManagerADT createSymbolTableManager();
void destroySymbolTableManager(SymbolTableManagerADT manager);
SymbolTableADT createSymbolTable();
void destroySymbolTable(SymbolTableADT table);

// Functions for reading information from the SymbolTable
void addItem(SymbolTableADT table, SymbolTableItem* item);
SymbolTableItem* getItemByID(const SymbolTableManagerADT table, const char* id, boolean recursive);
void freeSymbolTableItem(SymbolTableItem* item);
#endif
