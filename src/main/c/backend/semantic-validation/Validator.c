#include "Validator.h"

static boolean _validateExpression(Expression *expression, int *out);

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
            SymbolTableItem *item = getItemByID(_manager, factor->id);
            if(item == NULL){
                logError(_logger, "Symbol does not exists or is not accesible from current scope");
                return false;
            }
            if(item->type != INT){
                logError(_logger, "Expected an int but got another data type");
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
                logError(_logger, "zero division detected in expression");
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

/**Constant validation */
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

/**Simulation elements validation */
#pragma region ElementValidation

#pragma region ConnectionValidation
static SimulationNode *_getNodeReferenceItem(NodeReference *start){
    NodeReference *currentRef = start;

    if(currentRef == NULL){
        logError(_logger, "NULL NodeReference");
        return NULL;
    }

    SymbolTableItem *item = getItemByID(_manager, currentRef->reference);
    if(item == NULL){
        logError(_logger, "Symbol does not exists or is not accesible from current scope");
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

        item = getItemByID(_manager, currentRef->reference);
        if(item == NULL){
            logError(_logger, "Symbol does not exists or is not accesible from current scope");
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
        logError(_logger, "Invalid type. Expected Node or NodeTemplateInstance");
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

static boolean _validateNode(SimulationNode *node){
    
}

static boolean _validateInstance(TemplateInstance *instance){

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


static boolean _validateSimTemplate(SimulationTemplate* template){
    return _validateSimulationElements(template->simElements);
}

static boolean _validateSimulation(Simulation* simulation){

}


#pragma region ProgramValidation
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
