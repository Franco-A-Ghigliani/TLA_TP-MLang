#include "Computer.h"

#include <time.h>

#include "hashMap.h"
#include "../../shared/Logger.h"
#define DEFAULT_SIMULATION_NAME "My Simulation";
#define DEFAULT_SIMULATION_TIME_STEPS_LIMIT 100;
#define DEFAULT_SIMULATION_TIME_INTERVAL 1;

#define MIN_POSITION_X 46
#define MAX_POSITION_X 1518
#define MIN_POSITION_Y 69
#define MAX_POSITION_Y 713

static ComputationResult* computationResult;
static SymbolTableManagerADT symbolTableManager;
static Logger *_logger = NULL;
static HashMap * hashMap;

static int id = 100;
static int position = 0;

static int offsetX = 0;
static int offsetY = 0;
char * simulationTemplate = NULL;

static int _computeFactor(Factor* factor);
static int _computeExpression(Expression* expression);

static boolean _computeSimulation(Simulation* simulation);
static boolean _computeSimElements(SimElements* elements);
static NodeComputed* _computeNode(SimulationNode* node, char * originalId);

static int _computeFactor(Factor* factor) {
    switch (factor->type) {
    case FACTOR_STRING:
        return getItemByID(symbolTableManager, factor->id, true)->value.intValue;
    case INTEGER_TYPE:
        return factor->value;
    case EXPRESSION:
        int value = _computeExpression(factor->exp);
        if (factor->negated)
            value = -value;
        return value;
    }
}

static int _computeExpression(Expression* expression) {
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

static boolean _setParams(NodeComputed* nodeComputed, NodeParams* params) {
    logDebugging(_logger, "Setting parameters to nodeComputed");

    while (params != NULL) {
        NodeParam* param = params->nodeParam;
        switch (param->type) {
        case NODE_LABEL_TYPE:
            nodeComputed->name = param->string;
            break;
        case GATE_RANDOM_DISTRIBUTION_TYPE:
            nodeComputed->randomDistribution = param->boolean;
            break;
        case NODE_ACTIVATION_TYPE:
            nodeComputed->activation = param->activation;
            break;
        case NODE_ACTIVATION_MODE_TYPE:
            nodeComputed->activationMode = param->activationMode;
            break;
        case NODE_POSITION_TYPE:
            nodeComputed->positionX = _computeExpression(param->vector->x);
            nodeComputed->positionY = _computeExpression(param->vector->y);
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
            break;
        case DELAY_QUEUE_TYPE:
            nodeComputed->queue = param->boolean;
            break;
        case CONVERTER_MULTICONVERSION_TYPE:
            nodeComputed->multiConversion = param->boolean;
            break;
        case NODE_RESOURCE_COLOR_TYPE:
            nodeComputed->color = param->color;
            break;
        default:
            logError(_logger, "Unhandled node parameter of type %d", param->type);
            return false;
        }
        params = params->nextParams;
    }
    return true;
}

static boolean _computeTemplateInstantiation(TemplateInstance* instance) {
    switch (instance->type) {
    case SIMULATION_INSTANCE:
        offsetX = (rand() % MAX_POSITION_X + MIN_POSITION_X);
        offsetY = (rand() % MAX_POSITION_Y + MIN_POSITION_Y);
        simulationTemplate = instance->name;
        if (_computeSimElements(getItemByID(symbolTableManager, instance->templateReference, true)->value.simulationTemplate.elementsInTree) == false)
            return false;
        simulationTemplate = NULL;
        offsetX = 0;
        offsetY = 0;
        break;
    case NODE_INSTANCE:
        struct NodeInstance nodeInstance = getItemByID(symbolTableManager, instance->templateReference, true)->value.nodeInstance;
        NodeComputed* newItem = _computeNode(nodeInstance.originalTemplateInTree, instance->name);
        if (newItem == NULL)
            return false;
        if (_setParams(newItem, instance->nodeParams) == false)
            return false;
        break;
    default:
        logError(_logger, "Unhandled template type %d", instance->type);
        return false;
    }
    return true;
}


//Returns the new created NodeComputed
//originalId is used when computing node from template to avoid using the template id
static NodeComputed* _computeNode(SimulationNode* node, char * originalId) {
    logDebugging(_logger, "Computing simulation node of type %d and name %s", node->type, node->id);

    NodeComputed* nodeComputed = malloc(sizeof(NodeComputed));

    //Set id in case of nodeInstance
    char * nodeName;
    if (originalId != NULL)
         nodeName = originalId;
    else
         nodeName = node->id;

    //Set id in case of simulationInstance
    char name[150] = "";
    if (simulationTemplate != NULL) {
        strcpy(name, simulationTemplate);
        strcat(name, ".");
    }
    strcat(name, nodeName);

    logDebugging(_logger, "Final name for hashMap %s", name);

    nodeComputed->id = id;
    insert(hashMap, name, id);
    int found;
    logDebugging(_logger, "Set id %d to %s", get(hashMap, name, &found), name);
    id++;

    //Set defaults
    nodeComputed->name="";
    nodeComputed->initialResources=0;
    nodeComputed->capacityLimit=-1;
    nodeComputed->capacityDisplay=15;
    nodeComputed->queue=false;
    nodeComputed->drainOnOverflow=false;
    nodeComputed->multiConversion=false;
    nodeComputed->activation=PASSIVE;
    nodeComputed->activationMode=PULL_ANY;
    nodeComputed->color=BLACK;
    nodeComputed->positionX = rand() % MAX_POSITION_X + MIN_POSITION_X;
    nodeComputed->positionY = rand() % MAX_POSITION_Y + MIN_POSITION_Y;
    nodeComputed->randomDistribution=false;
    nodeComputed->layerPosition = position++;
    nodeComputed->next = NULL;

    NodeComputed** list = NULL;
    switch (node->type) {
    case SOURCE_TYPE:
        nodeComputed->activationMode = PUSH_ANY;
        nodeComputed->activation = AUTOMATIC;
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
    case END_CONDITION_TYPE:
        list = &computationResult->value->endConditions;
        break;
    case DELAY_TYPE:
        list = &computationResult->value->delays;
        break;
    default:
        logError(_logger, "Unknown node type");
        return NULL;
    }

    nodeComputed->next = *list;
    *list = nodeComputed;

    if (_setParams(nodeComputed, node->nodeParams) == false)
        return NULL;

    nodeComputed->positionX += offsetX;
    nodeComputed->positionY += offsetY;

    return nodeComputed;
}

void _concatenateReferences(NodeReference *head, char * buffer) {
    NodeReference * current = head;
    while (current != NULL) {
        strcat(buffer, current->reference);
        if (current->next != NULL)
            strcat(buffer, ".");
        current = current->next;
    }

}

static boolean _computeConnection(SimConnection* connection) {
    ConnectionComputed* connectionComputed = malloc(sizeof(ConnectionComputed));
    connectionComputed->id = id++;
    connectionComputed->layerPosition = position++;

    char from[50] = "";
    char to[50] =  "";
    _concatenateReferences(connection->from, from);
    _concatenateReferences(connection->to, to);

    int found;

    int sourceId = get(hashMap, from, &found);
    logDebugging(_logger, "Id from %s is %d", from ,sourceId);

    int targetId = get(hashMap, to, &found);
    logDebugging(_logger, "Id from %s is %d", to ,targetId);

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
        break;
    default:
        logError(_logger, "Unknown connection type");
        return false;
    }

    connectionComputed->next = *list;
    *list = connectionComputed;
    return true;
}



static boolean _computeSimElements(SimElements* elements) {
    logDebugging(_logger, "Computing simulation element of type %d", elements->type);
    switch (elements->type) {
    case CONNECTION:
        if (_computeConnection(elements->connection)==false)
            return false;
        break;
    case NODE_TYPE:
        if (_computeNode(elements->node, elements->node->id)==false)
            return false;
        break;
    case TEMPLATE_INSTANCIATION:
        if (_computeTemplateInstantiation(elements->templateInst) == false)
            return false;
        break;
    case NODE_TEMPLATE_TYPE:
    case EMPTY:
        //Nothing to compute
        break;
    default:
        logError(_logger, "Unknown simulation element type");
        return false;
    }

    if (elements->next != NULL)
        return _computeSimElements(elements->next);
    return true;
}

static boolean _computeSimParams(SimulationParams* params) {
    logDebugging(_logger, "Computing simulation parameters");

    for (int i = 0; i < 3 && params->params[i] != NULL; i++) {
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
            break;
        default:
            logError(_logger, "Unknown simulation parameter type");
            return false;
        }
    }
    return true;
}

static boolean _computeSimulation(Simulation* simulation) {
    logDebugging(_logger, "Computing main simulation");

    setSimulationScopeAsActive(symbolTableManager);
    if (_computeSimParams(simulation->params) == false)
        return false;

    srand(time(NULL));
    if (_computeSimElements(simulation->simElements)==false)
        return false;
    exitCurrentScope(symbolTableManager);
}

static boolean _computeSimulationWrapper(SimulationWrapper* simulationWrapper) {
    while (simulationWrapper->type != SIMULATION_TYPE && simulationWrapper->type != EMPTY_PROGRAM)
        simulationWrapper = simulationWrapper->nextSimulationWrapper;

    if (simulationWrapper->type == SIMULATION_TYPE)
        return _computeSimulation(simulationWrapper->simulation);

    return true;
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
void initializeComputerModule() {
    _logger = createLogger("Computer");

}

void shutdownComputerModule() {
    if (_logger != NULL) {
        destroyLogger(_logger);
    }
}


ComputationResult* compute(Program* program, SymbolTableManagerADT symbolTableManagerAdt) {
    ComputationResult* computed = malloc(sizeof(ComputationResult));
    computed->success = true;

    //Set defaults to simulation parameters
    SimulationComputed* simComputed = calloc(1,sizeof(SimulationComputed));
    SimulationParametersComputed * computedParameters = malloc(sizeof(SimulationParametersComputed));
    computedParameters->name = DEFAULT_SIMULATION_NAME;
    computedParameters->timeStepsLimit = DEFAULT_SIMULATION_TIME_STEPS_LIMIT;
    computedParameters->timeInterval = DEFAULT_SIMULATION_TIME_INTERVAL;
    simComputed->parameters = computedParameters;
    computed->value = simComputed;

    computationResult = computed;
    symbolTableManager = symbolTableManagerAdt;
    hashMap = createHashMap();

    _computeSimulationWrapper(program->simulationWrapper);

    freeHashMap(hashMap);
    return computationResult;
}

void destroyComputationResult(ComputationResult* computationResult) {
    free(computationResult->value->parameters);

    _destroyNodeComputedList(computationResult->value->sources);
    _destroyNodeComputedList(computationResult->value->pools);
    _destroyNodeComputedList(computationResult->value->gates);
    _destroyNodeComputedList(computationResult->value->converters);
    _destroyNodeComputedList(computationResult->value->drains);
    _destroyNodeComputedList(computationResult->value->delays);
    _destroyNodeComputedList(computationResult->value->endConditions);

    _destroyConnectionComputedList(computationResult->value->resourceConnections);
    _destroyConnectionComputedList(computationResult->value->stateConnections);

    free(computationResult->value);
    free(computationResult);
}
