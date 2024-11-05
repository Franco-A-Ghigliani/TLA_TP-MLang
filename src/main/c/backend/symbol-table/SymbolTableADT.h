#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"

typedef enum {INT, STRING, NODE, NODE_TEMPLATE, NODE_TEMPLATE_INSTANCE, SIM_TEMPLATE, SIM_TEMPLATE_INSTANCE} Types;

typedef struct Node
{
    NodeType type;

    struct commonParams
    {
        char * label;
        Vector position;
        boolean randomDistribution;
        Activation activation;
        ActivationMode activationMode;
        Color resourcesColor;
    };
    
    struct poolParams
    {
        int initialResources;
        Color initialResourcesColor;
        int capacity;
        int numberDisplayThreshold;
        boolean drainOnOverflow;
    };

    struct extraParams
    {
        boolean converterMulticonversion;
        boolean delayIsQueue;
        boolean gateIsRandomDistribution;
    };
} Node;

typedef union SymbolTableValues
{
    char* stringValue;
    int intValue;
    NodeType nodeValue;
    Node nodeTemplateValue;
    Node* templateInstanceParent;
    
} SymbolTableValues;


typedef struct SymbolTableItem
{
    char* id;
    Types type;

} SymbolTableItem;


#endif