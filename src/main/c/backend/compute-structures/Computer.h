#ifndef COMPUTER_H
#define COMPUTER_H

#include "../../shared/CompilerState.h"
#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../symbol-table/SymbolTableADT.h"

typedef struct NodeComputed {
    unsigned id;
    char * name;
    unsigned initialResources;
    int capacityLimit;
    int capacityDisplay;
    boolean drainOnOverflow;
    Activation activation;
    ActivationMode activationMode;
    Color color;
    ActivationMode mode;
    boolean distribution;

    struct NodeComputed * next;
} NodeComputed;

typedef struct ConnectionComputed {
    unsigned id;
    Formula formula;
    FormulaType formulaType;
    unsigned sourceId;
    unsigned targetId;

    struct ConnectionComputed * next;
}ConnectionComputed;

typedef struct SimulationComputed {
    char * name;
    unsigned timeInterval;
    unsigned timeStepsLimit;

    struct NodeComputed * sources;
    struct NodeComputed * pools;
    struct NodeComputed * gates;
    struct NodeComputed * converters;
    struct NodeComputed * drains;

    struct ConnectionComputed * connections;
} SimulationComputed;

typedef struct ComputationResult {
    boolean success;
    struct SimulationComputed * value;
} ComputationResult;

ComputationResult * compute(CompilerState * compilerState);
void destroyComputationResult(ComputationResult * computationResult);

#endif //COMPUTER_H
