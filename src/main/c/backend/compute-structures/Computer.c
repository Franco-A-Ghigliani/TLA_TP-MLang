#include "Computer.h"
#include "khash.h"
#include "../../shared/Logger.h"
#define DEFAULT_SIMULATION_NAME "My Simulation";
#define DEFAULT_SIMULATION_TIME_STEPS_LIMIT 100;
#define DEFAULT_SIMULATION_TIME_INTERVAL 1;

KHASH_MAP_INIT_STR(str_int, int)

static ComputationResult* computationResult;
static SymbolTableManagerADT symbolTableManager;
static Logger *_logger = NULL;

khash_t(str_int)* hashMap;
khiter_t hashMapIter;

static int id = 100;

int _computeFactor(Factor* factor);
int _computeExpression(Expression* expression);

int _computeFactor(Factor* factor) {
    switch (factor->type) {
    case FACTOR_STRING:
        return getItemByID(symbolTableManager, factor->id, true)->value.intValue;
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

NodeComputed* _computeNode(SimulationNode* node);

void _setParams(NodeComputed* nodeComputed, NodeParams* params) {
    logDebugging(_logger, "Setting parameters to nodeComputed");
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
}

void _computeTemplateInstantiation(TemplateInstance* instance) {
    struct NodeInstance nodeInstance = getItemByID(symbolTableManager, instance->templateReference, true)->value.
        nodeInstance;
    NodeComputed* newItem = _computeNode(nodeInstance.originalTemplateInTree);
    _setParams(newItem, instance->nodeParams);
}


//Returns the new created NodeComputed
NodeComputed* _computeNode(SimulationNode* node) {
    NodeComputed* nodeComputed = malloc(sizeof(NodeComputed));
    nodeComputed->id = id;
    int ret;
    hashMapIter = kh_put(str_int, hashMap, strdup(node->id), &ret);
    if (ret)
        kh_value(hashMap, hashMapIter) = id++;

    nodeComputed->next = NULL;

    _setParams(nodeComputed, node->nodeParams);

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
    default:
        logError(_logger, "Unknown node type");
    }

    nodeComputed->next = *list;
    *list = nodeComputed;

    return nodeComputed;
}

static void _computeConnection(SimConnection* connection) {
    ConnectionComputed* connectionComputed = malloc(sizeof(ConnectionComputed));
    connectionComputed->id = id++;

    int sourceId = 0;
    int targetId = 0;

    NodeReference* from = connection->from;
    while (from->next != NULL)
        from = from->next;
    hashMapIter = kh_get(str_int, hashMap, from->reference);
    if (hashMapIter != kh_end(hashMap))
        sourceId = kh_value(hashMap, hashMapIter);

    NodeReference* to = connection->to;
    while (to->next != NULL)
        to = to->next;
    hashMapIter = kh_get(str_int, hashMap, to->reference);
    if (hashMapIter != kh_end(hashMap))
        targetId = kh_value(hashMap, hashMapIter);

    connectionComputed->sourceId = sourceId;
    connectionComputed->targetId = targetId;
    connectionComputed->formula = connection->formula;

    ConnectionComputed** list = NULL;
    switch (connection->type) {
    case RESOURCE:
        list = &computationResult->value->resourceConnections;
        break;
    case STATE:
        list = &computationResult->value->stateConnections;
    }

    connectionComputed->next = *list;
    *list = connectionComputed;
}

static void _computeSimElements(SimElements* elements) {
    logDebugging(_logger, "Computing simulation element of type %d", elements->type);
    switch (elements->type) {
    case CONNECTION:
        _computeConnection(elements->connection);
        break;
    case NODE_TYPE:
        _computeNode(elements->node);
        break;
    case TEMPLATE_INSTANCIATION:
        _computeTemplateInstantiation(elements->templateInst);
    }
    if (elements->next != NULL)
        _computeSimElements(elements->next);
}

static void _computeSimParams(SimulationParams* params) {
    logDebugging(_logger, "Computing simulation parameters");
    for (int i = 0; i < 3; i++) {
        if (params->params[i] == NULL)
            break;
        SimulationParam* param = params->params[i];
        switch (param->type) {
        case NAME_PARAM:
            computationResult->value->parameters->name = param->string;
            break;
        case STEPS_PARAM:
            computationResult->value->parameters->timeStepsLimit = param->value;
            break;
        case STEP_INTERVAL_PARAM:
            computationResult->value->parameters->timeInterval = param->value;
        }
    }
}

static void _computeSimulation(Simulation* simulation) {
    logDebugging(_logger, "Computing main simulation");
    setSimulationScopeAsActive(symbolTableManager);
    logDebugging(_logger, "Set simulation scope");
    _computeSimParams(simulation->params);
    _computeSimElements(simulation->simElements);
    exitCurrentScope(symbolTableManager);
}

static void _computeSimulationWrapper(SimulationWrapper* simulationWrapper) {
    while (simulationWrapper->type != SIMULATION_TYPE && simulationWrapper->type != EMPTY_PROGRAM)
        simulationWrapper = simulationWrapper->nextSimulationWrapper;

    if (simulationWrapper->type == EMPTY_PROGRAM)
        return;
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
ComputationResult* compute(Program* program, SymbolTableManagerADT symbolTableManagerAdt) {
    ComputationResult* computed = malloc(sizeof(ComputationResult));

    SimulationComputed* simComputed = calloc(1,sizeof(SimulationComputed));
    SimulationParametersComputed * computedParameters = malloc(sizeof(SimulationParametersComputed));
    computedParameters->name = DEFAULT_SIMULATION_NAME;
    computedParameters->timeStepsLimit = DEFAULT_SIMULATION_TIME_STEPS_LIMIT;
    computedParameters->timeInterval = DEFAULT_SIMULATION_TIME_INTERVAL;
    simComputed->parameters = computedParameters;

    _logger = createLogger("Computer");
    computed->success = true;
    computed->value = simComputed;

    computationResult = computed;
    symbolTableManager = symbolTableManagerAdt;
    hashMap = kh_init(str_int);

    _computeSimulationWrapper(program->simulationWrapper);

    return computationResult;
}

void destroyComputationResult(ComputationResult* computationResult) {
    _destroyNodeComputedList(computationResult->value->sources);
    _destroyNodeComputedList(computationResult->value->pools);
    _destroyNodeComputedList(computationResult->value->gates);
    _destroyNodeComputedList(computationResult->value->converters);
    _destroyNodeComputedList(computationResult->value->drains);

    _destroyConnectionComputedList(computationResult->value->resourceConnections);
    _destroyConnectionComputedList(computationResult->value->stateConnections);

    free(computationResult->value);
    free(computationResult);
}
