#ifndef COMPUTER_H
#define COMPUTER_H

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../symbol-table/SymbolTableADT.h"

typedef struct NodeComputed {
    unsigned id;
    char * name;
    unsigned initialResources;
    int capacityLimit;
    int capacityDisplay;
    boolean queue;
    boolean drainOnOverflow;
    boolean multiConversion;
    Activation activation;
    ActivationMode activationMode;
    Color color;
    ActivationMode mode;
    boolean randomDistribution;
    int layerPosition;
    int positionX;
    int positionY;

    struct NodeComputed * next;
} NodeComputed;

typedef struct ConnectionComputed {
    unsigned id;
    Formula * formula;
    unsigned sourceId;
    unsigned targetId;
    int layerPosition;

    struct ConnectionComputed * next;
} ConnectionComputed;

typedef struct SimulationParametersComputed {
    char * name;
    unsigned timeInterval;
    unsigned timeStepsLimit;
} SimulationParametersComputed;

typedef struct SimulationComputed {
    struct SimulationParametersComputed * parameters;

    struct NodeComputed * sources;
    struct NodeComputed * pools;
    struct NodeComputed * gates;
    struct NodeComputed * converters;
    struct NodeComputed * drains;
    struct NodeComputed * endConditions;
    struct NodeComputed * delays;

    struct ConnectionComputed * resourceConnections;
    struct ConnectionComputed * stateConnections;
} SimulationComputed;

typedef struct ComputationResult {
    boolean success;
    struct SimulationComputed * value;
} ComputationResult;

struct CompilerState;

void initializeComputerModule();
void shutdownComputerModule();
ComputationResult* compute(Program* program, SymbolTableManagerADT symbolTableManagerAdt);
void destroyComputationResult(ComputationResult * computationResult);

#endif //COMPUTER_H
