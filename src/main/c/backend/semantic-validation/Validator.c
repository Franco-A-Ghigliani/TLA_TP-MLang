#include "Validator.h"

static boolean _validateExpression(Expression *expression, int *out);

typedef struct NodeAuxStructure
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
} NodeAuxStructure;


/* MODULE INTERNAL STATE */
const char _indentationCharacter = ' ';
const char _indentationSize = 4;
static Logger *_logger = NULL;
static boolean _status = true;
static SymbolTableManagerADT _manager = NULL;

void initializeValidatorModule()
{
    _logger = createLogger("Validator");
}

void shutdownValidatorModule()
{
    if (_logger != NULL)
    {
        destroyLogger(_logger);
    }
}

/** PRIVATE FUNCTIONS */

/**Expression Validation */
#pragma region ExpressionValidation
static boolean _validateFactor(Factor *factor, int *out){
    switch (factor->type)
    {
        case FACTOR_STRING:
            SymbolTableItem *item = getItemByID(_manager, factor->id, true);
            if(item == NULL){
                logError(_logger, "Symbol does not exist or is not accesible from current scope");
                return false;
            }
            if(item->type != INT){
                logError(_logger, "Invalid type. Expected int");
                return false;
            }
            *out = item->value.intValue;
            break;
        case INTEGER_TYPE:
            *out = factor->value;
            break;
        case EXPRESSION:
            if(!_validateExpression(factor->exp, out)){
                return false;
            }
            break;
        default:
            logError(_logger, "Invalid factor type");
            return false;
            break;
    }

    if(factor->negated){
        *out *= -1;
    }
    return true;
}

static boolean _validateExpression(Expression *expression, int *out){
    if(expression == NULL){
        logError(_logger, "NULL expresion");
        return false;
    }

    boolean isValid = true;
    switch (expression->type)
    {
        case ADDITION:
            int aux;
            isValid = _validateExpression(expression->leftExpression, out);
            if(!isValid)
                break;
            isValid = _validateExpression(expression->rightExpression, &aux);
            if(!isValid)
                break;
            *out += aux; 
            break;
        case SUBSTRACTION:
            int aux;
            isValid = _validateExpression(expression->leftExpression, out);
            if(!isValid)
                break;
            isValid = _validateExpression(expression->rightExpression, &aux);
            if(!isValid)
                break;
            *out -= aux; 
            break;
        case MULTIPLICATION:
            int aux;
            isValid = _validateExpression(expression->leftExpression, out);
            if(!isValid)
                break;
            isValid = _validateExpression(expression->rightExpression, &aux);
            if(!isValid)
                break;
            *out *= aux;
            break;
        case DIVISION:
            int aux;
            isValid = _validateExpression(expression->leftExpression, out);
            if(!isValid)
                break;
            isValid = _validateExpression(expression->rightExpression, &aux);
            if(!isValid)
                break;
            if(aux == 0){
                logError(_logger, "Zero division detected in expression");
                isValid = false;
                break;
            }
            *out /= aux;
            break;
        case FACTOR:
            isValid = _validateFactor(expression->factor, out);
            break;      
        default:
            break;
    }

    return isValid;
}
#pragma endregion

/**Simulation elements validation */
#pragma region ElementValidation

#pragma region ConnectionValidation
static SimulationNode *_getNodeReferenceItem(NodeReference *start){
    NodeReference *currentRef = start;

    if(currentRef == NULL){
        logError(_logger, "NULL NodeReference");
        return NULL;
    }

    SymbolTableItem *item = getItemByID(_manager, currentRef->reference, true);
    if(item == NULL){
        logError(_logger, "Symbol does not exist or is not accesible from current scope");
        return NULL;
    }

    int nestedScopes = 0;
    while (item->type == SIM_TEMPLATE_INSTANCE)
    {
        setNewActiveScope(_manager, item->value.simulationTemplateInstanceParent->internalSymbolTable);
        nestedScopes++;

        currentRef = currentRef->next;
        if(currentRef == NULL){
            logError(_logger, "NULL source");
            return NULL;
        }

        item = getItemByID(_manager, currentRef->reference, false);
        if(item == NULL){
            logError(_logger, "Symbol does not exist or is not accesible from current scope");
            return NULL;
        }
    }

    for (int i = 0; i < nestedScopes; i++)
    {
        exitCurrentScope(_manager);
    }

    if(item->type == NODE) {
        return item->value.nodeInTree;
    } else if(item->type == NODE_TEMPLATE_INSTANCE) {
        return item->value.nodeInstance.originalTemplateInTree;
    }else {
        logError(_logger, "Invalid type. Expected Node or instance of Template:Node");
        return NULL;
    }
}

static boolean _validateConnection(SimConnection *connection){
    if(connection == NULL){
        logError(_logger, "NULL connection");
        return false;
    }  

    SimulationNode *src = _getNodeReferenceItem(connection->from);
    SimulationNode *dest = _getNodeReferenceItem(connection->to);

    if(src == NULL || dest == NULL){
        logError(_logger, "NULL NodeReference in connection");
        return false;
    }

    if(connection->type == RESOURCE){
        if(dest->type == SOURCE_TYPE){
            logError(_logger, "Destination of resource connection can't be a Source node");
            return false;
        }
        if(src->type == DRAIN_TYPE){
            logError(_logger, "Source of resource connection can't be a Drain node");
            return false;

        }
        if(src->type == END_CONDITION_TYPE){
            logError(_logger, "EndCondition nodes can only be the destination of state connections");
            return false;
        }
    } else if(connection->type != STATE){
        logError(_logger, "Invalid connection type");
        return false;
    }

    if(connection->formula == NULL){
        logError(_logger, "NULL Formula in connection");
        return false;
    }
    int aux;
    return _validateExpression(connection->formula->expression, &aux);
}
#pragma endregion

static boolean _validateNodeParams(NodeParams *params, NodeType type){
    NodeParams *current = params;
    while (current != NULL)
    {
        switch (current->nodeParam->type)
        {
            case NODE_ACTIVATION_TYPE:
                if(type == END_CONDITION_TYPE)
                {
                    logError(_logger, "Activation is not valid in EndCondition nodes");
                    return false;
                }
                break;
            case NODE_ACTIVATION_MODE_TYPE:
                if(type == CONVERTER_TYPE || type == DELAY_TYPE || type == END_CONDITION_TYPE){
                    logError(_logger, "ActivationMode is not valid in this node");
                    return false;
                }
                if(type == SOURCE_TYPE && (current->nodeParam->activationMode == PULL_ALL || current->nodeParam->activationMode == PULL_ALL)){
                    logError(_logger, "Pull behaviours are not valid in Source nodes");
                    return false;
                }
                if((type == DRAIN_TYPE || type == GATE_TYPE) 
                && (current->nodeParam->activationMode == PUSH_ALL || current->nodeParam->activationMode == PUSH_ANY)){
                    logError(_logger, "Push behaviours are not valid in this node");
                    return false;
                }
                break;
            case POOL_INITIAL_RESOURCES_TYPE:
                if(type != POOL_TYPE){
                    logError(_logger, "InitalResources is not valid in this node");
                    return false;
                }
                int aux;
                if(!_validateExpression (current->nodeParam->expression, &aux)){
                    return false;
                }
                break;
            case NODE_RESOURCE_COLOR_TYPE:
                if(type != SOURCE_TYPE && type != CONVERTER_TYPE){
                    logError(_logger, "ResourceColor is not valid in this node");
                    return false;
                }
                break;
            case POOL_INITIAL_RESOURCES_COLOR_TYPE:
                if(type != POOL_TYPE){
                    logError(_logger, "InitalResourcesColor is not valid in this node");
                    return false;
                }
                break;
            case POOL_CAPACITY_TYPE:
                if(type != POOL_TYPE){
                    logError(_logger, "Capacity is not valid in this node");
                    return false;
                }
                int aux;
                if(!_validateExpression (current->nodeParam->expression, &aux)){
                    return false;
                }
                break;
            case POOL_NUMBER_DISPLAY_THRESHOLD_TYPE:
                if(type != POOL_TYPE){
                    logError(_logger, "NumberDisplayThreshold is not valid in this node");
                    return false;
                }
                int aux;
                if(!_validateExpression (current->nodeParam->expression, &aux)){
                    return false;
                }
                break;
            case GATE_RANDOM_DISTRIBUTION_TYPE:
                if(type != GATE_TYPE){
                    logError(_logger, "RandomDistribution is not valid in this node");
                    return false;
                }
                break;
            case POOL_DRAIN_ON_OVERFLOW_TYPE:
                if(type != POOL_TYPE){
                    logError(_logger, "DrainOnOverflow is not valid in this node");
                    return false;
                }
                break;
            case CONVERTER_MULTICONVERSION_TYPE:
                if(type != CONVERTER_TYPE){
                    logError(_logger, "Multiconversion is not valid in this node");
                    return false;
                }
                break;
            case DELAY_QUEUE_TYPE:
                if(type != DRAIN_TYPE){
                    logError(_logger, "Queue is not valid in this node");
                    return false;
                }
                break;
            default:
                break;
        }
        current = current->nextParams;
    }
    
    return true;
}

static boolean _validateNode(SimulationNode *node){
    if(node == NULL){
        logError(_logger, "NULL node");
        return false;
    }
    
    if(!_validateNodeParams(node->nodeParams, node->type)){
        logError(_logger, "Invalid node parameters");
        return false;
    }
    
    SymbolTableItem *newItem = calloc(1, sizeof(SymbolTableItem));
    newItem->type = node->isTemplate? NODE_TEMPLATE : NODE;
    newItem->id = node->id;
    newItem->value.nodeInTree = node;
    
    addItem(getActiveScope(_manager), newItem);
    return true;
}

static boolean _validateInstance(TemplateInstance *instance){
    if(instance == NULL){
        logError(_logger, "NULL instance");
        return false;
    }

    if(getItemByID(_manager, instance->name, true) != NULL){
        logError(_logger, "Symbol already existant in this scope");
        return false;
    }

    SymbolTableItem *refItem = getItemByID(_manager, instance->templateReference, true);
    if(refItem == NULL){
        logError(_logger, "Symbol does not exist or is not accesible from current scope");
        return false;
    }

    if(refItem->type == NODE_TEMPLATE){
        boolean isValid = _validateNodeParams(instance->nodeParams, refItem->value.nodeInTree->type);
        if(isValid){
            SymbolTableItem *newItem = calloc(1, sizeof(SymbolTableItem));
            newItem->id = instance->name;
            newItem->type = NODE_TEMPLATE_INSTANCE;
            newItem->value.nodeInstance.instanceOverridesinTree = instance->nodeParams;
            newItem->value.nodeInstance.originalTemplateInTree = refItem->value.nodeInTree;
            addItem(getActiveScope(_manager), newItem);
        }
        return isValid;
    } else if(refItem->type != SIM_TEMPLATE){
        logError(_logger, "Invalid type. Expected Template:Node or Template:Simulation");
        return false;
    }

    SymbolTableItem *newItem = calloc(1, sizeof(SymbolTableItem));
    newItem->id = instance->name;
    newItem->type = SIM_TEMPLATE_INSTANCE;
    newItem->value.simulationTemplateInstanceParent = &(refItem->value.simulationTemplate);
    addItem(getActiveScope(_manager), newItem);

}

static boolean _validateSimulationElements(SimElements *elements){
    if(elements == NULL){
        logError(_logger, "NULL element");
        return false;
    }

    boolean isValid = true;

    switch (elements->type)
    {
        case CONNECTION:
            isValid = _validateConnection(elements->connection);
            break;
        case NODE_TYPE || NODE_TEMPLATE_TYPE:
            isValid = _validateNode(elements->node);
            break;
        case TEMPLATE_INSTANCIATION:
            isValid = _validateInstance(elements->templateInst);
            break;
        case EMPTY:
            return true;
            break;
        default:
        logError(_logger, "Invalid element type");
            break;
    }

    if(isValid){
        return _validateSimulationElements(elements->next);
    }
    return false;
}
#pragma endregion

#pragma region SimulationValidation
static boolean _validateSimulationParams(SimulationParams *params){
    boolean foundName = false, foundSteps = false, foundInterval = false;
    for (size_t i = 0; i < 3; i++)
    {
        switch (params->params[i]->type)
        {
            case NAME_PARAM:
                if(foundName){
                    logError(_logger, "Repeated simulation parameter");
                    return false;
                }
                foundName = true;
                break;
            case STEP_INTERVAL_PARAM:
                if(foundInterval){
                    logError(_logger, "Repeated simulation parameter");
                    return false;
                }
                foundInterval = true;
                break;
            case STEPS_PARAM:
                if(foundSteps){
                    logError(_logger, "Repeated simulation parameter");
                    return false;
                }
                foundSteps = true;
                break;
        }
    }
}

static boolean _validateSimulation(Simulation* simulation){
    if(simulation == NULL){
        logError(_logger, "NULL Simulation");
        return false;
    }

    if(!_validateSimulationParams(simulation->params)){
        logError(_logger, "Invalid simulation parameters");
        return false;
    }

    return _validateSimulationElements(simulation->simElements);
}
#pragma endregion

#pragma region ProgramValidation

static boolean _validateConstant(Constant *constant){
    if(constant == NULL){
        logError(_logger, "NULL constant");
        return false;
    }

    SymbolTableItem *newItem = calloc(1, sizeof(SymbolTableItem));

    if(newItem == NULL){
        logError(_logger, "Error allocating table item");
        return false;
    }

    if(constant->constantName == NULL){
        logError(_logger, "NULL constant name");
        return false;
    }
    newItem->id = constant->constantName;

    if(constant->type == VALUE_EXPRESSION){
        newItem->type = INT;
        if(_validateExpression(constant->expression, &(newItem->value.intValue)) == false){
            return false;
        }
    } else if (constant->type == VALUE_STRING){
        newItem->type = STRING;
        if(constant->string == NULL){
            logError(_logger, "NULL string pointer");
            return false;
        }
        newItem->value.stringValue = constant->string;
    } else {
        logError(_logger, "Invalid constant type");
        return false;
    }
    addItem(getActiveScope(_manager), newItem);
    return true;
}

static boolean _validateSimTemplate(SimulationTemplate* template){
    if(template == NULL){
        logError(_logger, "NULL Template:Simulation");
        return false;
    }
    SymbolTableADT newTable = createSymbolTable();

    SymbolTableItem *newItem = calloc(1, sizeof(SymbolTableItem));
    newItem->type = SIM_TEMPLATE;
    newItem->id = template->name;
    newItem->value.simulationTemplate.internalSymbolTable = newTable;
    addItem(getActiveScope(_manager), newItem);

    setNewActiveScope(_manager, newTable);
    boolean valid = _validateSimulationElements(template->simElements);
    exitCurrentScope(_manager);
    return valid;
}

static boolean _validateSimWrappers(SimulationWrapper* simWrapper){
    if(simWrapper == NULL){
        return true;
    }

    boolean isValid;

    switch (simWrapper->type)
    {
        case CONSTANT:
            isValid = _validateConstant(simWrapper->constant);
            break;
        case SIMULATION_TEMPLATE:
            isValid = _validateSimTemplate(simWrapper->simulationTemplate);
            break;
        case SIMULATION_TYPE:
            return _validateSimulation(simWrapper->simulation);
            break;
        case EMPTY_PROGRAM:
            return true;
            break;
        default:
            logError(_logger, "Invalid wrapper type");
            return false;
            break;
    }

    if(isValid){
        return _validateSimWrappers(simWrapper->nextSimulationWrapper);
    }
    return false;
}

static boolean _generateTables(Program* program)
{
    return _validateSimWrappers(program->simulationWrapper);
}
#pragma endregion

/** PUBLIC FUNCTION */
boolean validate(CompilerState *compilerState)
{
    Program *program = compilerState->abstractSyntaxtTree;

    logDebugging(_logger, "Generating and vaildating symbol table...");
    if(program == NULL){
        logError(_logger, "NULL program. Invalid syntax tree.");
        _status = false;
        return _status;
    }

    _manager = compilerState->symbolTables;

    _status = _generateTables(program);
    
    if(!_status){
        logDebugging(_logger, "Invalid semantics.");
    } else {
        logDebugging(_logger, "Valid semantics.");
    }
    logDebugging(_logger, "Validation is done.");
    return _status;
}
