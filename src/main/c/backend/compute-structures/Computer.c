#include "Computer.h"

static ComputationResult* computationResult;
static SymbolTableManagerADT symbolTableManager;
static unsigned id = 100;

int _computeFactor(Factor* factor);
int _computeExpression(Expression* expression);

int _computeFactor(Factor* factor) {
    switch (factor->type) {
    case FACTOR_STRING:
        return getItemByID(symbolTableManager, factor->id)->value.intValue;
    case INTEGER_TYPE:
        return factor->value;
    case EXPRESSION:
        return _computeExpression(factor->exp);
    }
}

int _computeExpression(Expression* expression) {
    int leftValue, rightValue;
    switch (expression->type) {
    case ADDITION:
        leftValue = _computeExpression(expression->leftExpression);
        rightValue = _computeExpression(expression->rightExpression);
        return leftValue + rightValue;
    case SUBSTRACTION:
        leftValue = _computeExpression(expression->leftExpression);
        rightValue = _computeExpression(expression->rightExpression);
        return leftValue - rightValue;
    case MULTIPLICATION:
        leftValue = _computeExpression(expression->leftExpression);
        rightValue = _computeExpression(expression->rightExpression);
        return leftValue * rightValue;
    case DIVISION:
        leftValue = _computeExpression(expression->leftExpression);
        rightValue = _computeExpression(expression->rightExpression);
        return leftValue / rightValue;
    case FACTOR:
        return _computeFactor(expression->factor);
    }
}

void _computeTemplateInstantiation(TemplateInstance* instance) {}

static void _computeNode(SimulationNode* node) {
    if (node->isTemplate)
        return;

    NodeComputed* nodeComputed = malloc(sizeof(NodeComputed));
    nodeComputed->id = id++;
    nodeComputed->next = NULL;

    NodeParams* params = node->nodeParams;
    while (params != NULL) {
        NodeParam* param = params->nodeParam;
        switch (param->type) {
        case NODE_LABEL_TYPE:
            nodeComputed->name = param->string;
            break;
        case GATE_RANDOM_DISTRIBUTION_TYPE:
            nodeComputed->distribution = param->boolean;
            break;
        case NODE_ACTIVATION_TYPE:
            nodeComputed->activation = param->activation;
            break;
        case NODE_ACTIVATION_MODE_TYPE:
            nodeComputed->activationMode = param->activationMode;
            break;
        case POOL_INITIAL_RESOURCES_TYPE:
            nodeComputed->initialResources = _computeExpression(param->expression);
            break;
        case POOL_INITIAL_RESOURCES_COLOR_TYPE:
            nodeComputed->color = param->color;
            break;
        case POOL_CAPACITY_TYPE:
            nodeComputed->capacityLimit = _computeExpression(param->expression);
            break;
        case POOL_NUMBER_DISPLAY_THRESHOLD_TYPE:
            nodeComputed->capacityDisplay = _computeExpression(param->expression);
            break;
        case POOL_DRAIN_ON_OVERFLOW_TYPE:
            nodeComputed->drainOnOverflow = param->boolean;
        }
        params = params->nextParams;
    }

    NodeComputed** list = NULL;
    switch (node->type) {
    case SOURCE_TYPE:
        list = &computationResult->value->sources;
        break;
    case POOL_TYPE:
        list = &computationResult->value->pools;
        break;
    case GATE_TYPE:
        list = &computationResult->value->gates;
        break;
    case CONVERTER_TYPE:
        list = &computationResult->value->converters;
        break;
    case DRAIN_TYPE:
        list = &computationResult->value->drains;
        break;
    }

    if (*list == NULL) {
        *list = nodeComputed;
    }
    else {
        nodeComputed->next = *list;
        *list = nodeComputed;
    }
}

static void _computeConnection(SimConnection* connection) {}

static void _computeSimElements(SimElements* elements) {
    switch (elements->type) {
    case CONNECTION:
        _computeConnection(elements->connection);
        break;
    case NODE:
        _computeNode(elements->node);
        break;
    case TEMPLATE_INSTANCIATION:
        _computeTemplateInstantiation(elements->templateInst);
    }
}

static void _computeSimParams(SimulationParams* params) {
    for (int i = 0; i < 3; i++) {
        SimulationParam* param = params->params[i];
        switch (param->type) {
        case NAME_PARAM:
            computationResult->value->name = param->string;
            break;
        case STEPS_PARAM:
            computationResult->value->timeStepsLimit = param->value;
            break;
        case STEP_INTERVAL_PARAM:
            computationResult->value->timeInterval = param->value;
        }
    }
}

static void _computeSimulation(Simulation* simulation) {
    _computeSimParams(simulation->params);
    _computeSimElements(simulation->simElements);
}

static void _computeSimulationWrapper(SimulationWrapper* simulationWrapper) {
    while (simulationWrapper->type != SIMULATION_TYPE)
        simulationWrapper = simulationWrapper->nextSimulationWrapper;

    _computeSimulation(simulationWrapper->simulation);
}

static void _destroyNodeComputedList(struct NodeComputed* head) {
    struct NodeComputed* current = head;
    while (current != NULL) {
        struct NodeComputed* next = current->next;
        free(current);
        current = next;
    }
}

static void _destroyConnectionComputedList(struct ConnectionComputed* head) {
    struct ConnectionComputed* current = head;
    while (current != NULL) {
        struct ConnectionComputed* next = current->next;
        free(current);
        current = next;
    }
}

//------------------------Public Functions-----------------------
ComputationResult* compute(CompilerState* compilerState) {
    ComputationResult* computed = malloc(sizeof(ComputationResult));
    SimulationComputed* simComputed = malloc(sizeof(SimulationComputed));

    computed->success = true;
    computed->value = simComputed;

    computationResult = computed;
    symbolTableManager = compilerState->symbolTables;

    _computeSimulationWrapper(((Program*)compilerState->abstractSyntaxtTree)->simulationWrapper);

    return computationResult;
}

void destroyComputationResult(ComputationResult* computationResult) {
    _destroyNodeComputedList(computationResult->value->sources);
    _destroyNodeComputedList(computationResult->value->pools);
    _destroyNodeComputedList(computationResult->value->gates);
    _destroyNodeComputedList(computationResult->value->converters);
    _destroyNodeComputedList(computationResult->value->drains);

    _destroyConnectionComputedList(computationResult->value->connections);

    free(computationResult->value);
    free(computationResult);
}
