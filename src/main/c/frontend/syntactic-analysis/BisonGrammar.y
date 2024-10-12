%{

#include "BisonActions.h"

%}

// You touch this, and you die.
%define api.value.union.name SemanticValue

%union {
	/** Terminals. */

	int integer;
	Token token;
	char* string;
	Activation activation;
	ActivationMode activationMode;
	Color color;
	boolean bool;

	/** Non-terminals. */

	Constant * constant;
	Expression * expression;
	Factor * factor;
	Program * program;
	Vector * vector;
	Simulation * simulation;
	SimulationWrapper * simWrapper;
	TemplateInstance * templateInstance;
	SimulationParams * simParams;
	SimulationParam * simParam;
	SimulationName * simName;
	SimulationStepsToSimulate * simStepsToSimulate;
	SimulationStepInterval * simStepInterval;
	SimulationNode * simNode;
	SimulationTemplate * simTemplate;
	GenericValue * genericValue;
	SimElements * simElements;
	SimConnection * simConnection;
	NodeParams * nodeParams;
	NodeParam * nodeParam;
	NodeReference * nodeReference;
	Formula * formula;
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach. To use this mechanism, the AST must be translated into
 * another structure.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */
/*
%destructor { releaseConstant($$); } <constant>
%destructor { releaseExpression($$); } <expression>
%destructor { releaseFactor($$); } <factor>
%destructor { releaseProgram($$); } <program>
*/

/** Terminals. */

/** CONNECTION*/
%token <token> RESOURCE_CONNECT
%token <token> STATE_CONNECT
%token <token> PERIOD

/**FORMULA*/
%token <token> LESS_THAN
%token <token> GREATER_THAN
%token <token> PERCENTAGE
%token <token> ADD
%token <token> SUBSTRACT
%token <token> MULTIPLY
%token <token> DIVIDE

/**INITIAL*/
%token <token> SIMULATION 
%token <token> CONSTANT_KEY

/**INITIAL + SIM_NODES*/
%token <token> TEMPLATE
%token <token> COLON

/*NODE_PARAM_VALUE*/
%token <activation> NODE_ACTIVATION_ENUM
%token <color> COLOR
%token <activationMode> NODE_ACTIVATION_MODE_ENUM
%token <bool> BOOLEAN
/*NODE_PARAM_VALUE + CONNECTION + INSTANCIATION + CONST*/
%token <token> SEMI_COLON
/*NODE_PARAM_VALUE + SIM_PARAM_VALUE*/
%token <string> STRING
/*NODE_PARAM_VALUE, SIM_PARAM_VALUE, VECTOR, FORMULA, CONST*/
%token <integer> INTEGER

/*NODE_PARAMS*/
%token <token> NODE_LABEL
%token <token> NODE_POSITION
%token <token> NODE_ACTIVATION 
%token <token> NODE_RESOURCE_COLOR 
%token <token> NODE_ACTIVATION_MODE 
%token <token> POOL_INITIAL_RESOURCES
%token <token> POOL_INITIAL_RESOURCES_COLOR
%token <token> POOL_CAPACITY
%token <token> POOL_NUMBER_DISPLAY_THRESHOLD
%token <token> POOL_DRAIN_ON_OVERFLOW
%token <token> GATE_RANDOM_DISTRIBUTION
%token <token> CONVERTER_MULTICONVERSION
%token <token> DELAY_QUEUE
/*NODE_PARAMS + SIM_PARAMS*/
%token <token> EQUALS

/*SIM_NODES*/
%token <token> SOURCE
%token <token> DRAIN
%token <token> POOL
%token <token> GATE
%token <token> CONVERTER
%token <token> DELAY
%token <token> END_CONDITION
%token <token> NEW
/*SIM_NODES + FORMULA + NODE_PARAMS*/
%token <token> CLOSE_BRACKET

/*SIM_PARAM_VALUE + VECTOR*/
%token <token> COMMA

/*SIM_PARAMS*/
%token <token> SIMULATION_NAME 
%token <token> STEP_INTERVAL
%token <token> STEPS_TO_SIMULATE
/*SIM_PARAMS + FORMULA + VECTOR*/
%token <token> CLOSE_PARENTHESIS 

/*SIM + CONNECTION + INSTANCIATION + NODE*/
%token <token> OPEN_BRACKET
/*SIM + NODE_PARAM_VALUE + FORMULA*/
%token <token> OPEN_PARENTHESIS
/*SIM + NODE + INSTANCIATION + SIM_NODES + CONNECTION + FORMULA*/
%token <string> ID

%token <token> UNKNOWN

/** TODO: Non-terminals. */
%type <program> program
%type <simulation> simulation
%type <simWrapper> simWrapper
%type <simParams> simParams
%type <simParam> simParam
%type <simNode> simNode
%type <vector> vector
%type <simTemplate> simTemplate
%type <templateInstance> templateInstance
%type <constant> constant
%type <simElements> simElements
%type <simConnection> simConnection
%type <nodeParams> nodeParams
%type <nodeParam> nodeParam
%type <nodeReference> nodeReference
%type <formula> formula
%type <expression> expression
%type <factor> factor
/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left ADD SUBSTRACT
%left MULTIPLY DIVIDE

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.


//----------------------------------------SIM WRAPPER--------------------------------------
//EVALUAR SI QUIERO EL EMPTY, YA QUE PODRIAN INGRESAR PROGRAMAS VACIOS O SIN SIMULATION PRINCIPAL
//PERO ASI SE ACEPTA CUALQUIER COMBINACION Y ORDEN
program:	simWrapper[wrap]															{$$ = wrapperProgramSemanticAction(currentCompilerState(), $wrap);}
	;

simWrapper: constant[const] simWrapper[wrap]											{$$ = constantWrapperSemanticAction($const, $wrap);}
	|		simTemplate[temp] simWrapper[wrap]											{$$ = templateWrapperSemanticAction($temp, $wrap);}
	|		simulation[sim] 															{$$ = simulationWrapperSemanticAction($sim);}
	|		%empty																		{$$ = emptyWrapperSemanticAction();}
	;



constant: 	CONSTANT_KEY ID[id] EQUALS expression[exp]									{$$ = expressionConstantSemanticAction($id, $exp);}
	|		CONSTANT_KEY ID[id] EQUALS STRING[str]										{$$ = stringConstantSemanticAction($id, $str);}
	;

simTemplate: TEMPLATE COLON SIMULATION ID[id] OPEN_PARENTHESIS CLOSE_PARENTHESIS OPEN_BRACKET simElements[elems] CLOSE_BRACKET	{$$ = templateSemanticAction($id, $elems);}
	;

simulation: SIMULATION OPEN_PARENTHESIS simParams[params] CLOSE_PARENTHESIS OPEN_BRACKET simElements[elems] CLOSE_BRACKET		{$$ = simulationSemanticAction($params, $elems);}
	;

simElements: simNode[node] simElements[elems]  											{$$ = nodeElementsSemanticAction($node, $elems);}
	| TEMPLATE COLON simNode[node] simElements[elems]									{$$ = nodeTemplateElementsSemanticAction($node, $elems);}
	| templateInstance[inst]	simElements[elems]										{$$ = intanciationElementsSemanticAction($inst, $elems);}
	| simConnection[connection] simElements[elems]										{$$ = connectionElementsSemanticAction($connection, $elems);}
	| %empty																			{$$ = emptyElementsSemanticAction();}
	;

templateInstance: NEW ID[id] ID[instanceId] OPEN_BRACKET nodeParams[params] CLOSE_BRACKET			{$$ = nodeInstanciationSemanticAction($id, $instanceId, $params);}
	| NEW ID[id] ID[instanceId] SEMI_COLON															{$$ = simInstanciationSemanticAction($id, $instanceId);}
//---------------------------------------------------------------------------------------
//------------------------------SIM PARAMS-----------------------------------------------

simParams: simParam[param1] COMMA simParam[param2] COMMA simParam[param3] 				{$$ = simParamsSemanticAction($param1, $param2, $param3);}
	|	simParam[param1] COMMA simParam[param2] 										{$$ = simParamsSemanticAction($param1, $param2, NULL);}
	|	simParam[param] 																{$$ = simParamsSemanticAction($param, NULL, NULL);}
	|	%empty																			{$$ = simParamsSemanticAction(NULL, NULL, NULL);}
	;

simParam: SIMULATION_NAME EQUALS STRING[name]											{$$ = nameSimParamSemanticAction($name);}
	|	STEPS_TO_SIMULATE EQUALS INTEGER[steps]											{$$ = stepsToSimulateSimParamSemanticAction($steps);}
	|	STEP_INTERVAL EQUALS INTEGER[interval]											{$$ = stepIntervalSimParamSemanticAction($interval);}
	;

//----------------------------------------------------------------------------------------
//-------------------------------------SIM NODE-------------------------------------------
simNode: SOURCE ID[id] OPEN_BRACKET nodeParams[params] CLOSE_BRACKET								{$$ = nodeSemanticAction($id, $params, SOURCE);}
	| GATE ID[id] OPEN_BRACKET nodeParams[params] CLOSE_BRACKET										{$$ = nodeSemanticAction($id, $params, GATE);}
	| DRAIN ID[id] OPEN_BRACKET nodeParams[params] CLOSE_BRACKET									{$$ = nodeSemanticAction($id, $params, DRAIN);}
	| POOL ID[id] OPEN_BRACKET nodeParams[params] CLOSE_BRACKET										{$$ = nodeSemanticAction($id, $params, POOL);}
	| CONVERTER ID[id] OPEN_BRACKET nodeParams[params] CLOSE_BRACKET								{$$ = nodeSemanticAction($id, $params, CONVERTER);}
	| DELAY ID[id] OPEN_BRACKET nodeParams[params] CLOSE_BRACKET									{$$ = nodeSemanticAction($id, $params, DELAY);}
	| END_CONDITION ID[id] OPEN_BRACKET nodeParams[params] CLOSE_BRACKET							{$$ = nodeSemanticAction($id, $params, END_CONDITION);}
	;

nodeParams: nodeParam[param] 																	{$$ = nodeParamsSemanticAction($param, NULL);}
	| nodeParam[param]	SEMI_COLON nodeParams[next]												{$$ = nodeParamsSemanticAction($param, $next);}
	;

nodeParam: NODE_LABEL EQUALS STRING[val]														{$$ = labelParamSemanticAction($val);}
	| NODE_POSITION EQUALS vector[val]															{$$ = positionParamSemanticAction($val);}
	| NODE_ACTIVATION EQUALS NODE_ACTIVATION_ENUM[val]											{$$ = nodeActivationParamSemanticAction($val);}
	| NODE_ACTIVATION_MODE EQUALS NODE_ACTIVATION_MODE_ENUM[val]								{$$ = activationModeParamSemanticAction($val);}
	| NODE_RESOURCE_COLOR EQUALS COLOR[val]														{$$ = resourceColorParamSemanticAction($val);}
	| POOL_INITIAL_RESOURCES EQUALS expression[val]												{$$ = initialResourcesParamSemanticAction($val);}
	| POOL_INITIAL_RESOURCES_COLOR EQUALS COLOR[val]											{$$ = initialResourcesColorParamSemanticAction($val);}
	| POOL_CAPACITY EQUALS expression[val]														{$$ = capacityParamSemanticAction($val);}
	| POOL_NUMBER_DISPLAY_THRESHOLD EQUALS expression[val]										{$$ = numberDisplayThresholdParamSemanticAction($val);}
	| POOL_DRAIN_ON_OVERFLOW EQUALS BOOLEAN[val]												{$$ = drainOnOverflowParamSemanticAction($val);}
	| GATE_RANDOM_DISTRIBUTION EQUALS BOOLEAN[val]												{$$ = randomDistParamSemanticAction($val);}
	| CONVERTER_MULTICONVERSION EQUALS BOOLEAN[val]												{$$ = multiconversionParamSemanticAction($val);}
	| DELAY_QUEUE EQUALS BOOLEAN[val]															{$$ = queueParamSemanticAction($val);}
	;

//--------------------------------------------------------------------------------------------
//---------------------------------------SIM CONNECTION---------------------------------------
simConnection: nodeReference[from] RESOURCE_CONNECT nodeReference[to] OPEN_BRACKET formula[form] CLOSE_BRACKET		{$$ = connectionSemanticAction($from, $to, $form, RESOURCE);}
	| nodeReference[from] STATE_CONNECT nodeReference[to] OPEN_BRACKET formula[form] CLOSE_BRACKET					{$$ = connectionSemanticAction($from, $to, $form, STATE);}
	;

formula: LESS_THAN expression[exp]																		{$$ = arithmeticFormulaSemanticAction($exp, LESS_THAN);}
	| GREATER_THAN expression[exp]																		{$$ = arithmeticFormulaSemanticAction($exp, GREATER_THAN);}
	| expression[exp] PERCENTAGE 																		{$$ = arithmeticFormulaSemanticAction($exp, LESS_THAN);}
	| expression[exp] 																					{$$ = arithmeticFormulaSemanticAction($exp, FORMULA_EXPRESSION);}
	;

expression: expression[left] SUBSTRACT expression[right]														{$$ = arithmeticExpressionSemanticAction($left, $right, SUBTRACTION);}
	| expression[left] ADD expression[right]																	{$$ = arithmeticExpressionSemanticAction($left, $right, ADDITION);}
	| expression[left] MULTIPLY expression[right]																{$$ = arithmeticExpressionSemanticAction($left, $right, MULTIPLICATION);}
	| expression[left] DIVIDE expression[right]	 																{$$ = arithmeticExpressionSemanticAction($left, $right, DIVISION);}
	| factor																									{$$ = factorExpressionSemanticAction($1);}
	;

factor: ID																				{$$ = idFactorSemanticAction($1);}
	| INTEGER																			{$$ = integerFactorSemanticAction($1);}
	;

nodeReference: ID 																		{$$ = nodeReferenceSemanticAction($1, NULL);} 
	| ID PERIOD nodeReference 															{$$ = nodeReferenceSemanticAction($1, $3);}
	;
	
//----------------------------------------------------------------------------------------
//-------------------------------VECTOR---------------------------------------------------
vector: OPEN_PARENTHESIS INTEGER[x] COMMA INTEGER[y] CLOSE_PARENTHESIS					{$$ = vectorSemanticAction($x, $y);}
	;
%%
