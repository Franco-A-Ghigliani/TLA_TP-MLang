#include "Generator.h"

#include <math.h>
#include <time.h>

#include "../symbol-table/SymbolTableADT.h"

#define DEFAULT_LAYER 80
#define DEFAULT_NUMBER_OF_RUNS 10
#define CSV_NAME "result.csv"

static Logger* _logger = NULL;
SymbolTableManagerADT manager = NULL;
static FILE* csvFile = NULL;



/** PRIVATE FUNCTIONS */

/**
 * Creates the prologue of the generated output, a Latex document that renders
 * a tree thanks to the Forest package.
 *
 * @see https://ctan.dcc.uchile.cl/graphics/pgf/contrib/forest/forest-doc.pdf
 */
static void _generatePrologue(SimulationComputed* simulation) {
    logDebugging(_logger, "Generating prologue");

    fprintf(csvFile, "\n");
    fprintf(csvFile, "DIAGRAM PROPERTIES\n");
    fprintf(csvFile, ",Name:,%s\n", simulation->parameters->name);
    fprintf(csvFile, ",URL:\n");
    fprintf(csvFile, ",Owner:\n");

    time_t now = time(NULL);
    struct tm* localTime = localtime(&now);
    char dateBuffer[100];
    strftime(dateBuffer, sizeof(dateBuffer), "%m/%d/%Y %H:%M:%S", localTime);
    fprintf(csvFile, ",Creation Date:,%s\n", dateBuffer);
    fprintf(csvFile, ",Last Change:,%s\n", dateBuffer);

    fprintf(csvFile, ",Time Interval:,%u\n", simulation->parameters->timeInterval);
    fprintf(csvFile, ",Time Steps Limit:,%u\n", simulation->parameters->timeStepsLimit);
    fprintf(csvFile, ",Number of Runs (total):,%d\n", DEFAULT_NUMBER_OF_RUNS);
    fprintf(csvFile, ",Exporting Codec:,v2.0\n");
    fprintf(csvFile, "\n");
}

static void _generateEpilogue() {
    logDebugging(_logger, "Generating epilogue");

    fprintf(csvFile, "LAYERS\n");
    fprintf(csvFile, "ID,Label,Parent Layer ID,Visible,Locked\n");
    fprintf(csvFile, "%d,,,true,false\n", DEFAULT_LAYER-1);
    fprintf(csvFile, "%d,,%d,true,false\n", DEFAULT_LAYER, DEFAULT_LAYER-1);
    fprintf(csvFile,"\n");
}

// ------------------------------------------AUXILIARY FUNCTIONS---------------------------------------------
static char * _vectorToString(int x, int y) {
    char auxBuffer[100];
    snprintf(auxBuffer, sizeof(auxBuffer), "\"{\"\"x\"\":%d,\"\"y\"\":%d,\"\"width\"\":46,\"\"height\"\":46}\"", x, y);
    return strdup(auxBuffer);
}

static char* _activationToString(Activation activation) {
    switch (activation) {
    case AUTOMATIC:
        return "automatic";
    case ON_START:
        return "onstart";
    case PASSIVE:
        return "passive";
    case INTERACTIVE:
        return "interactive";
    }
}

static char* _colorToString(Color color) {
    switch (color) {
    case BLACK:
        return "Black";
    case RED:
        return "Red";
    case GREEN:
        return "Green";
    case BLUE:
        return "Blue";
    case ORANGE:
        return "Orange";
    }
}

static char * _booleanToString(boolean boolean) {
    if (boolean)
        return "true";
    return "false";
}



static char* _activationModeToString(ActivationMode activationMode) {
    switch (activationMode) {
    case PUSH_ALL:
        return "push-all";
    case PUSH_ANY:
        return "push-any";
    case PULL_ALL:
        return "pull-all";
    case PULL_ANY:
        return "pull-any";
    }
}

static char* _drainOnOverflowToString(boolean overflow) {
    if (overflow)
        return "drain";
    return "block";
}

static char * _distributionToString(boolean distribution) {
    if (distribution)
        return "dice";
    return "deterministic";
}

static char * _multiconversionToString(boolean isMulticonversion) {
    if (isMulticonversion)
        return "multiple";
    return "single";
}

static char* _formulaTypeToString(FormulaType type) {
    switch (type) {
    case PERCENTAGE_TYPE: return "%";
    case GREATER_THAN_TYPE: return ">";
    case LESS_THAN_TYPE: return "<";
    case FORMULA_EXPRESSION: return "";
    }
}

static void _formulaToString(Formula * formula, char * buffer, size_t bufferSize) {
    const char *typeString = _formulaTypeToString(formula->type);
    snprintf(buffer, bufferSize, "%s%d", typeString, formula->expression->factor->value);
}

//-------------------------------------------GENERATE PRIVATE FUNCTIONS-----------------------------------------------------

static void _generateSources(NodeComputed* source) {
    fprintf(csvFile, "%u,%s,%u,,%s,,%s,%s,%s,%d\n", source->id, source->name, DEFAULT_LAYER, _vectorToString(source->positionX, source->positionY),
            _activationToString(source->activation), _colorToString(source->color),
            _activationModeToString(source->activationMode), source->layerPosition);
    if (source->next != NULL)
        _generateSources(source->next);
}

static void _generatePools(NodeComputed* pool) {
    fprintf(csvFile, "%u,%s,%u,,%s,,%s,%s,%u,%s,%d,%d,%s,true,%d\n", pool->id, pool->name, DEFAULT_LAYER, _vectorToString(pool->positionX, pool->positionY),
            _activationToString(pool->activation), _activationModeToString(pool->activationMode), pool->initialResources,
            _colorToString(pool->color), pool->capacityLimit, pool->capacityDisplay,
            _drainOnOverflowToString(pool->drainOnOverflow), pool->layerPosition);
    if (pool->next != NULL)
        _generatePools(pool->next);
}

static void _generateGates(NodeComputed* gate) {
    fprintf(csvFile, "%u,%s,%u,,%s,,%s,%s,%s,%d\n", gate->id, gate->name, DEFAULT_LAYER, _vectorToString(gate->positionX, gate->positionY),_activationToString(gate->activation),_activationModeToString(gate->activationMode), _distributionToString(gate->randomDistribution), gate->layerPosition);
    if (gate->next != NULL)
        _generateGates(gate->next);
}

static void _generateConverters(NodeComputed * converter) {
    fprintf(csvFile, "%u,%s,%u,,%s,,%s,%s,%s,%s,%d\n", converter->id, converter->name, DEFAULT_LAYER, _vectorToString(converter->positionX, converter->positionY),_activationToString(converter->activation), _activationModeToString(converter->activationMode), _colorToString(converter->color), _multiconversionToString(converter->multiConversion), converter->layerPosition);
    if (converter->next != NULL)
        _generateConverters(converter->next);
}

static void _generateDrains(NodeComputed * drain) {
    fprintf(csvFile, "%u,%s,%u,,%s,,%s,%s,%d\n", drain->id, drain->name, DEFAULT_LAYER, _vectorToString(drain->positionX, drain->positionY),_activationToString(drain->activation), _activationModeToString(drain->activationMode), drain->layerPosition);
    if (drain->next != NULL)
        _generateDrains(drain->next);
}

static void _generateDelays(NodeComputed * delay) {
    fprintf(csvFile, "%u,%s,%u,,%s,,%s,%s,%d\n", delay->id, delay->name, DEFAULT_LAYER, _vectorToString(delay->positionX, delay->positionY),_activationToString(delay->activation), _booleanToString(delay->queue), delay->layerPosition);
    if (delay->next != NULL)
        _generateDelays(delay->next);
}

static void _generateEndConditions(NodeComputed * endCondition) {
    fprintf(csvFile, "%u,%s,%u,,%s,,%d\n", endCondition->id, endCondition->name, DEFAULT_LAYER, _vectorToString(endCondition->positionX, endCondition->positionY), endCondition->layerPosition);
    if (endCondition->next != NULL)
        _generateEndConditions(endCondition->next);
}

static void _generateResourceConnections(ConnectionComputed * resourceConnections) {
    char formula[64];
    _formulaToString(resourceConnections->formula, formula, sizeof(formula));
    fprintf(csvFile, "%u,,%u,,%s,,%s,,%u,%u,interval-based,false,Black,false,,,%d\n", resourceConnections->id, DEFAULT_LAYER, _vectorToString(0,0),formula, resourceConnections->sourceId, resourceConnections->targetId, resourceConnections->layerPosition);
    if (resourceConnections->next != NULL)
        _generateResourceConnections(resourceConnections->next);
}

static void _generateStateConnections(ConnectionComputed * stateConnections) {
    char formula[64];
    _formulaToString(stateConnections->formula, formula, sizeof(formula));
    fprintf(csvFile, "%u,,%u,,%s,,%s,%u,%u,false,Black,receiving resource,%d\n", stateConnections->id, DEFAULT_LAYER, _vectorToString(0,0),formula, stateConnections->sourceId, stateConnections->targetId, stateConnections->layerPosition);
    if (stateConnections->next != NULL)
        _generateStateConnections(stateConnections->next);
}

/**
 * Generates the output of the program.
 */
static void _generateProgram(SimulationComputed* simulation) {
    logDebugging(_logger, "Generating main simulation");

    if (simulation->sources != NULL) {
        logDebugging(_logger, "Generating sources");
        fprintf(csvFile, "SOURCES\n");
        fprintf(csvFile,
                "ID,Label,Layer ID,Group ID,Geometry,Style,Activation,Resources (color),Activation Mode,Position\n");
        _generateSources(simulation->sources);
        fprintf(csvFile, "\n");
    }

    if (simulation->drains != NULL) {
        logDebugging(_logger, "Generating drains");
        fprintf(csvFile, "DRAINS\n");
        fprintf(csvFile, "ID,Label,Layer ID,Group ID,Geometry,Style,Activation,Activation Mode,Position\n");
        _generateDrains(simulation->drains);
        fprintf(csvFile, "\n");
    }

    if (simulation->pools != NULL) {
        logDebugging(_logger, "Generating pools");
        fprintf(csvFile, "POOLS\n");
        fprintf(csvFile,
                "ID,Label,Layer ID,Group ID,Geometry,Style,Activation,Activation Mode,Resources,Resources (color),Capacity (limit),Capacity (display),Overflow,Show in chart,Position\n");
        _generatePools(simulation->pools);
        fprintf(csvFile, "\n");
    }

    if (simulation->gates != NULL) {
        logDebugging(_logger, "Generating gates");
        fprintf(csvFile, "GATES\n");
        fprintf(csvFile, "ID,Label,Layer ID,Group ID,Geometry,Style,Activation,Activation Mode,Distribution,Position\n");
        _generateGates(simulation->gates);
        fprintf(csvFile, "\n");
    }

    if (simulation->converters != NULL) {
        logDebugging(_logger, "Generating converters");
        fprintf(csvFile, "CONVERTERS\n");
        fprintf(csvFile, "ID,Label,Layer ID,Group ID,Geometry,Style,Activation,Activation Mode,Resources (color),Conversion,Position\n");
        _generateConverters(simulation->converters);
        fprintf(csvFile, "\n");
    }

    if (simulation->delays != NULL) {
        logDebugging(_logger, "Generating delays");
        fprintf(csvFile, "DELAYS\n");
        fprintf(csvFile, "ID,Label,Layer ID,Group ID,Geometry,Style,Activation,Queue,Position\n");
        _generateDelays(simulation->delays);
        fprintf(csvFile, "\n");
    }

    if (simulation->endConditions != NULL) {
        logDebugging(_logger, "Generating end conditions");
        fprintf(csvFile, "END CONDITIONS\n");
        fprintf(csvFile, "ID,Label,Layer ID,Group ID,Geometry,Style,Position\n");
        _generateEndConditions(simulation->endConditions);
        fprintf(csvFile, "\n");
    }


    if (simulation->resourceConnections != NULL) {
        logDebugging(_logger, "Generating resource connections");
        fprintf(csvFile, "RESOURCE CONNECTIONS\n");
        fprintf(csvFile, "ID,Label,Layer ID,Group ID,Geometry,Style,Formula,Interval,Source,Target,Transfer,Color Coding,Color Coding (color),Shuffle Source,Limits (minimum),Limits (maximum),Position\n");
        _generateResourceConnections(simulation->resourceConnections);
        fprintf(csvFile, "\n");
    }
    if (simulation->stateConnections != NULL) {
        logDebugging(_logger, "Generating state connections");
        fprintf(csvFile, "STATE CONNECTIONS\n");
        fprintf(csvFile, "ID,Label,Layer ID,Group ID,Geometry,Style,Formula,Source,Target,Color Coding,Color Coding (color),Trigger on,Position\n");
        _generateStateConnections(simulation->stateConnections);
        fprintf(csvFile, "\n");
    }
}


// /** PUBLIC FUNCTIONS */
void initializeGeneratorModule() {
    _logger = createLogger("Generator");

}

void shutdownGeneratorModule() {
    if (_logger != NULL) {
        destroyLogger(_logger);
    }
}

void generate(CompilerState* compilerState) {
    logDebugging(_logger, "Generating final output...");

    FILE* file = fopen(CSV_NAME, "w");
    if (file == NULL) {
        logError(_logger, "Failed to open file %s", CSV_NAME);
        return;
    }
    csvFile = file;

    _generatePrologue(compilerState->simulation);
    _generateProgram(compilerState->simulation);
    _generateEpilogue();

    fclose(csvFile);
    logDebugging(_logger, "Generation is done.");
}
