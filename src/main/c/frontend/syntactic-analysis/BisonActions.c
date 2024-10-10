#include "BisonActions.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeBisonActionsModule() {
	_logger = createLogger("BisonActions");
}

void shutdownBisonActionsModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** IMPORTED FUNCTIONS */

extern unsigned int flexCurrentContext(void);

/* PRIVATE FUNCTIONS */

static void _logSyntacticAnalyzerAction(const char * functionName);

/**
 * Logs a syntactic-analyzer action in DEBUGGING level.
 */
static void _logSyntacticAnalyzerAction(const char * functionName) {
	logDebugging(_logger, "%s", functionName);
}

/* PUBLIC FUNCTIONS */


Program * ExpressionProgramSemanticAction(CompilerState * compilerState) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	compilerState->abstractSyntaxtTree = program;
	if (0 < flexCurrentContext()) {
		logError(_logger, "The final context is not the default (0): %d", flexCurrentContext());
		compilerState->succeed = false;
	}
	else {
		compilerState->succeed = true;
	}
	return program;
}

Factor * IntegerFactorSemanticAction(int value){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = calloc(1, sizeof(Factor));
	factor ->value = value;
	factor->type = INTEGER;
	return factor;

}

NodeReference * NodeReferenceSemanticAction(char * id, NodeReference * next){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	NodeReference * nodeReference = calloc(1, sizeof(NodeReference));
	nodeReference ->reference = id;
	nodeReference ->next=next;
	return nodeReference;
}

Vector * VectorSemanticAction(int x, int y){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Vector * vector = calloc(1, sizeof(Vector));
	vector->x = x;
	vector->y = y;
	return vector;
}