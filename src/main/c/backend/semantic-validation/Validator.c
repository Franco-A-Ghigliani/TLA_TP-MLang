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

}

static boolean _validateSimulation(Simulation* simulation){

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
