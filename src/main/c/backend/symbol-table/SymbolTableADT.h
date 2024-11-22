#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"

typedef struct SymbolTableManager* SymbolTableManagerADT;
typedef struct SymbolTable* SymbolTableADT;

typedef enum
{
    INT,
    STRING,
    NODE,
    NODE_TEMPLATE,
    NODE_TEMPLATE_INSTANCE,
    SIM_TEMPLATE,
    SIM_TEMPLATE_INSTANCE,
    BOTTOM
} SymbolTableValueType;

typedef struct SymbolTableItem
{
    char *id;
    SymbolTableValueType type;
    SymbolTableValue value;
} SymbolTableItem;

typedef struct NodeSymbol
{
    NodeType type;

    struct commonParams
    {
        char *label;
        Vector position;
        Activation activation;
        ActivationMode activationMode;
        Color resourcesColor;
    } commonParams;

    struct poolParams
    {
        int initialResources;
        Color initialResourcesColor;
        int capacity;
        int numberDisplayThreshold;
        boolean drainOnOverflow;
    } poolParams;

    struct extraParams
    {
        boolean converterMulticonversion;
        boolean delayIsQueue;
        boolean gateIsRandomDistribution;
    } extraParams;
} NodeSymbol;

typedef struct SimulationTemplateSymbol
{
    SymbolTableADT internalSymbolTable;
} SimulationTemplateSymbol;

typedef union SymbolTableValue
{
    char *stringValue;
    int intValue;
    NodeType nodeValue;

    NodeSymbol nodeTemplateValue;
    NodeSymbol *nodeTemplateInstanceParent;

    SimulationTemplateSymbol simulationTemplate;
    SimulationTemplateSymbol *simulationTemplateInstanceParent;
} SymbolTableValue;

void exitCurrentScope(SymbolTableManagerADT manager);
SymbolTableADT getActiveScope(const SymbolTableManagerADT manager);
void setNewActiveScope(SymbolTableManagerADT manager, SymbolTableADT newTable);
SymbolTableADT getNextScope(const SymbolTableADT activeScope);
void resetScope(SymbolTableManagerADT manager);

// Functions to create and destroy symbol table structures
SymbolTableManagerADT createSymbolTableManager();
void destroySymbolTableManager(SymbolTableManagerADT manager);
SymbolTableADT createSymbolTable();
void destroySymbolTable(SymbolTableADT table);

// Functions for reading information from the SymbolTable
void addItem(SymbolTableADT table, SymbolTableItem* item);
SymbolTableItem* getItemByID(const SymbolTableManagerADT table, const char* id);
void freeSymbolTableItem(SymbolTableItem* item);
#endif