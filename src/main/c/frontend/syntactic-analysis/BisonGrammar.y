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
%type <simParams> simParams
%type <simParam> simParam
%type <simNode> simNode
%type <vector> vector
%type <simTemplate> simTemplate
%type <constant> constant
%type <simElements> simElements
%type <simConnection> simConnection
%type <nodeParams> nodeParams
%type <nodeReference> nodeReference
%type <formula> formula
/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left ADD SUB
%left MUL DIV

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.


//----------------------------------------SIM WRAPPER--------------------------------------
//EVALUAR SI QUIERO EL EMPTY, YA QUE PODRIAN INGRESAR PROGRAMAS VACIOS O SIN SIMULATION PRINCIPAL
//PERO ASI SE ACEPTA CUALQUIER COMBINACION Y ORDEN
program:	simWrapper																	{}
	;

simWrapper: constant simWrapper														    {}
	|		simTemplate simWrapper														{}
	|		simulation simWrapper														{}
	|		%empty																		{}
	;

constant: 	CONSTANT_KEY ID EQUALS INTEGER												{}
	|		CONSTANT_KEY ID EQUALS STRING												{}
	;

simTemplate: TEMPLATE COLON SIMULATION ID OPEN_PARENTHESIS CLOSE_PARENTHESIS OPEN_BRACKET simElements CLOSE_BRACKET	{}
	;

simulation: SIMULATION OPEN_PARENTHESIS simParams CLOSE_PARENTHESIS OPEN_BRACKET simElements CLOSE_BRACKET					{}
	;

simElements: simNode simElements  														{}
	| TEMPLATE COLON simNode simElements												{}
	| simConnection simElements															{}
	| %empty																			{}
	;
//---------------------------------------------------------------------------------------
//------------------------------SIM PARAMS-----------------------------------------------
simParams: simParam COMMA simParam COMMA simParam 										{}
	|	simParam COMMA simParam 														{}
	|	simParam 																		{}
	|	%empty																			{}
	;

simParam: SIMULATION_NAME EQUALS STRING[name]											{}
	|	STEPS_TO_SIMULATE EQUALS INTEGER[steps]											{}
	|	STEP_INTERVAL EQUALS INTEGER[interval]											{}
	;

//----------------------------------------------------------------------------------------
//-------------------------------------SIM NODE-------------------------------------------
simNode: NEW ID OPEN_BRACKET nodeParams CLOSE_BRACKET									{}
	| SOURCE ID OPEN_BRACKET nodeParams CLOSE_BRACKET									{}
	| GATE ID OPEN_BRACKET nodeParams CLOSE_BRACKET										{}
	| DRAIN ID OPEN_BRACKET nodeParams CLOSE_BRACKET									{}
	| POOL ID OPEN_BRACKET nodeParams CLOSE_BRACKET										{}
	| CONVERTER ID OPEN_BRACKET nodeParams CLOSE_BRACKET								{}
	| DELAY ID OPEN_BRACKET nodeParams CLOSE_BRACKET									{}
	| END_CONDITION ID OPEN_BRACKET nodeParams CLOSE_BRACKET							{}
	;

nodeParams: nodeParam 																	{}
	| nodeParam	SEMI_COLON nodeParams													{}
	;

nodeParam: NODE_LABEL EQUALS STRING														{}
	| NODE_POSITION EQUALS vector														{}
	| GATE_RANDOM_DISTRIBUTION EQUALS BOOLEAN											{}
	| NODE_ACTIVATION EQUALS NODE_ACTIVATION_ENUM										{}
	| NODE_ACTIVATION_MODE EQUALS NODE_ACTIVATION_MODE_ENUM								{}
	| POOL_INITIAL_RESOURCES EQUALS INTEGER												{}
	| NODE_RESOURCE_COLOR EQUALS COLOR													{}
	| POOL_INITIAL_RESOURCES_COLOR EQUALS COLOR											{}
	| POOL_CAPACITY EQUALS INTEGER														{}
	| POOL_NUMBER_DISPLAY_THRESHOLD EQUALS INTEGER										{}
	| POOL_DRAIN_ON_OVERFLOW EQUALS BOOLEAN												{}
	| CONVERTER_MULTICONVERSION EQUALS BOOLEAN											{}
	| DELAY_QUEUE EQUALS BOOLEAN														{}
	;

//--------------------------------------------------------------------------------------------
//---------------------------------------SIM CONNECTION---------------------------------------
simConnection: nodeReference RESOURCE_CONNECT nodeReference OPEN_BRACKET formula CLOSE_BRACKET		{}
	| nodeReference STATE_CONNECT nodeReference OPEN_BRACKET formula CLOSE_BRACKET					{}
	;

formula: LESS_THAN formula																{}
	| GREATER_THAN formula																{}
	| INTEGER formula																	{}
	| ID formula																		{}
	| PERCENTAGE formula																{}
	| SUBSTRACT formula																	{}
	| ADD formula																		{}
	| MULTIPLY formula 																	{}
	| DIVIDE formula																	{}
	| ID 																				{}
	| INTEGER																			{}
	;

nodeReference: ID 																		{} 
	| ID PERIOD nodeReference 															{}
	;
	
//----------------------------------------------------------------------------------------
//-------------------------------VECTOR---------------------------------------------------
vector: OPEN_PARENTHESIS INTEGER[x] COMMA INTEGER[y] CLOSE_PARENTHESIS					{$$ = VectorSemanticAction($x, $y);}
	;
%%
