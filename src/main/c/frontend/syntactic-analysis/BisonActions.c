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


Program * wrapperProgramSemanticAction(CompilerState * compilerState, SimulationWrapper* wrapper) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->simulationWrapper = wrapper;
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

SimulationWrapper* constantWrapperSemanticAction(Constant* constant, SimulationWrapper* next){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	SimulationWrapper * wrapper = calloc(1, sizeof(SimulationWrapper));
	wrapper->name = constant;
	wrapper->type= CONSTANT;
	wrapper->nextSimulationWrapper=next;
	return wrapper;
}

SimulationWrapper* templateWrapperSemanticAction(SimulationTemplate* template, SimulationWrapper* next){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	SimulationWrapper * wrapper = calloc(1, sizeof(SimulationWrapper));
	wrapper->simulationTemplate = template;
	wrapper->type= SIMULATION_TEMPLATE;
	wrapper->nextSimulationWrapper=next;
	return wrapper;
}

SimulationWrapper* simulationWrapperSemanticAction(Simulation* sim){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	SimulationWrapper * wrapper = calloc(1, sizeof(SimulationWrapper));
	wrapper->simulation = sim;
	wrapper->type= SIMULATION;
	wrapper->nextSimulationWrapper = NULL;
	return wrapper;
}

SimulationWrapper* emptyWrapperSemanticAction(){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	SimulationWrapper * wrapper = calloc(1, sizeof(SimulationWrapper));
	wrapper->type = EMPTY_PROGRAM;
	return wrapper;
}

Constant* expressionConstantSemanticAction(char* id, Expression* exp){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Constant * constant = calloc(1, sizeof(Constant));
	constant->constantName=id;
	constant->expression=exp;
	constant->type=VALUE_EXPRESSION;
	return constant;
}

Constant* stringConstantSemanticAction(char* id, char* str){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Constant * constant = calloc(1, sizeof(Constant));
	constant->constantName=id;
	constant->string=str;
	constant->type=VALUE_STRING;
	return constant;
}

SimulationTemplate* templateSemanticAction(char* id, SimElements* elements){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	SimulationTemplate * template = calloc(1, sizeof(SimulationTemplate));
	template->simElements=elements;
	template->name=id;
	return template; 
}


Simulation* simulationSemanticAction(SimulationParams* params, SimElements* elements){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Simulation * sim = calloc(1, sizeof(Simulation));
	sim->params = params;
	sim->simElements = elements;
	return sim;
}

SimElements* nodeElementsSemanticAction(SimulationNode* node, SimElements* next){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	SimElements * sim = calloc(1, sizeof(SimElements));
	sim->node = node;
	sim->type=NODE;
	sim->next = next;
	return sim;
}

SimElements* nodeTemplateElementsSemanticAction(SimulationNode* node, SimElements* next){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	SimElements* simElems = calloc(1, sizeof(SimElements));

	node->isTemplate = true;
	simElems->node = node;
	simElems->type = NODE_TEMPLATE;
	simElems->next = NULL;
	return simElems;
}

SimElements* intanciationElementsSemanticAction(TemplateInstanciate* inst, SimElements* next){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	SimElements* simElems = calloc(1, sizeof(SimElements));

	simElems->templateInst = inst;
	simElems->type = TEMPLATE_INSTANCIATION;
	simElems->next = next;
	return simElems;
}

SimElements* connectionElementsSemanticAction(SimConnection* connection, SimElements* next){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	SimElements* simElems = calloc(1, sizeof(SimElements));

	simElems->connection = connection;
	simElems->type = CONNECTION;
	simElems->next = next;
	return simElems;	
}

SimElements* emptyElementsSemanticAction(){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	SimElements* simElems = calloc(1, sizeof(SimElements));

	simElems->type = EMPTY;
	return simElems;
}

TemplateInstanciate* nodeInstanciationSemanticAction(char* templateId, char* instanceId, NodeParams* params){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TemplateInstanciate* templateInstanciate = calloc(1, sizeof(TemplateInstanciate));

	templateInstanciate->templateReference = templateId;
	templateInstanciate->name = instanceId;
	templateInstanciate->nodeParams=params;
	templateInstanciate->type=NODE_INSTANCE;
	return templateInstanciate;
}

TemplateInstanciate* simInstanciationSemanticAction(char* templateId, char* instanceId){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TemplateInstanciate* templateInstanciate = calloc(1, sizeof(TemplateInstanciate));

	templateInstanciate->templateReference = templateId;
	templateInstanciate->name = instanceId;
	templateInstanciate->type=SIMULATION_INSTANCE;
	return templateInstanciate;
}

SimulationParams* simParamsSemanticAction(SimulationParam* param1, SimulationParam* param2, SimulationParam* param3){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	SimulationParams* simulationParams = calloc(1, sizeof(SimulationParams));

	simulationParams->param1=param1;
	simulationParams->param2=param2;
	simulationParams->param3=param3;
	return simulationParams;
}

SimulationParam* nameSimParamSemanticAction(char* name){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	SimulationParam* simulationParam = calloc(1, sizeof(SimulationParam));

	simulationParam->string = name;
	simulationParam->type = NAME; 
	return simulationParam;	
}

SimulationParam* stepsToSimulateSimParamSemanticAction(int steps){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	SimulationParam* simulationParam = calloc(1, sizeof(SimulationParam));

	simulationParam->value = steps;
	simulationParam->type = STEPS; 
	return simulationParam;	
}

SimulationParam* stepIntervalSimParamSemanticAction(int interval){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	SimulationParam* simulationParam = calloc(1, sizeof(SimulationParam));

	simulationParam->value = interval;
	simulationParam->type = STEP_INTERVAL; 
	return simulationParam;	
}

SimulationNode* nodeSemanticAction(char* id,  NodeParams* params, NodeType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	SimulationNode* node = calloc(1, sizeof(SimulationNode));

	node->isTemplate = false;
	node->nodeParams = params;
	node->type = type;
	node->id = id;
	return node;
}

NodeParams* nodeParamsSemanticAction(NodeParam* param, NodeParams* next){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	NodeParams* nodeParams = calloc(1, sizeof(NodeParams));

	nodeParams->nodeParam=param;
	nodeParams->nextParams=next;
	return nodeParams;
}

NodeParam* labelParamSemanticAction(char* label){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	NodeParam* nodeParam = calloc(1, sizeof(NodeParam));

	nodeParam->string=label;
	nodeParam->type=NODE_LABEL;
	return nodeParam;	
}

NodeParam* positionParamSemanticAction(Vector* vector){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	NodeParam* nodeParam = calloc(1, sizeof(NodeParam));

	nodeParam->vector=vector;
	nodeParam->type=NODE_POSITION;
	return nodeParam;	
}

NodeParam* nodeActivationParamSemanticAction(Activation activation){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	NodeParam* nodeParam = calloc(1, sizeof(NodeParam));

	nodeParam->activation=activation;
	nodeParam->type=NODE_ACTIVATION;
	return nodeParam;
}

NodeParam* activationModeParamSemanticAction(ActivationMode mode){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	NodeParam* nodeParam = calloc(1, sizeof(NodeParam));

	nodeParam->activationMode = mode;
	nodeParam->type=NODE_ACTIVATION_MODE;
	return nodeParam;	
}

NodeParam* resourceColorParamSemanticAction(Color color){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	NodeParam* nodeParam = calloc(1, sizeof(NodeParam));

	nodeParam->color = color;
	nodeParam->type=NODE_RESOURCE_COLOR;
	return nodeParam;	
}

NodeParam* initialResourcesParamSemanticAction(Expression* exp){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	NodeParam* nodeParam = calloc(1, sizeof(NodeParam));

	nodeParam->expression = exp;
	nodeParam->type=POOL_INITIAL_RESOURCES;
	return nodeParam;	
}

NodeParam* initialResourcesColorParamSemanticAction(Color color){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	NodeParam* nodeParam = calloc(1, sizeof(NodeParam));

	nodeParam->color = color;
	nodeParam->type = POOL_INITIAL_RESOURCES_COLOR;
	return nodeParam;		
}

NodeParam* capacityParamSemanticAction(Expression* exp){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	NodeParam* nodeParam = calloc(1, sizeof(NodeParam));

	nodeParam->expression = exp;
	nodeParam->type = POOL_CAPACITY;
	return nodeParam;
}

NodeParam* numberDisplayThresholdParamSemanticAction(Expression* exp){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	NodeParam* nodeParam = calloc(1, sizeof(NodeParam));

	nodeParam->expression = exp;
	nodeParam->type = POOL_NUMBER_DISPLAY_THRESHOLD;
	return nodeParam;	
}

NodeParam* drainOnOverflowParamSemanticAction(boolean val){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	NodeParam* nodeParam = calloc(1, sizeof(NodeParam));

	nodeParam->boolean = val;
	nodeParam->type = POOL_DRAIN_ON_OVERFLOW;
	return nodeParam;
}

NodeParam* randomDistParamSemanticAction(boolean val){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	NodeParam* nodeParam = calloc(1, sizeof(NodeParam));

	nodeParam->boolean = val;
	nodeParam->type = GATE_RANDOM_DISTRIBUTION;
	return nodeParam;	
}

NodeParam* multiconversionParamSemanticAction(boolean val){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	NodeParam* nodeParam = calloc(1, sizeof(NodeParam));

	nodeParam->boolean = val;
	nodeParam->type = CONVERTER_MULTICONVERSION;
	return nodeParam;	
}

NodeParam* queueParamSemanticAction(boolean val){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	NodeParam* nodeParam = calloc(1, sizeof(NodeParam));

	nodeParam->boolean = val;
	nodeParam->type = DELAY_QUEUE;
	return nodeParam;		
}

SimConnection* connectionSemanticAction(NodeReference* src, NodeReference* dest, Formula* formula, ConnectionType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	SimConnection* simConnection = calloc(1, sizeof(SimConnection));

	simConnection->from = src;
	simConnection->to=dest;
	simConnection->formula=formula;
	simConnection->type=type;
	return simConnection;			
}

Formula* arithmeticFormulaSemanticAction(Expression* expression, FormulaType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Formula* formula = calloc(1, sizeof(Formula));

	formula->expression=expression;
	formula->type=type;
	return formula;	
}

Expression* arithmeticExpressionSemanticAction(Expression* left, Expression* right, ExpressionType type){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression* expression = calloc(1, sizeof(Expression));

	expression->leftExpression=left;
	expression->rightExpression=right;
	expression->type=type;
	return expression;		
}

Expression* factorExpressionSemanticAction(Factor * factor){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression* expression = calloc(1, sizeof(Expression));

	expression->factor=factor;
	expression->type=FACTOR;
	return expression;		
}

Factor* idFactorSemanticAction(char* id){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor* factor = calloc(1, sizeof(Factor));

	factor->id=id;
	factor->type=FACTOR_STRING;
	return factor;		
}

Factor* integerFactorSemanticAction(int val){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor* factor = calloc(1, sizeof(Factor));

	factor->value=val;
	factor->type=INTEGER;
	return factor;		
}

NodeReference * nodeReferenceSemanticAction(char * id, NodeReference * next){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	NodeReference * nodeReference = calloc(1, sizeof(NodeReference));
	nodeReference ->reference = id;
	nodeReference ->next=next;
	return nodeReference;
}

Vector * vectorSemanticAction(int x, int y){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Vector * vector = calloc(1, sizeof(Vector));
	vector->x = x;
	vector->y = y;
	return vector;
}