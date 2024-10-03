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
	SimulationParams * simParams;
	SimulationParam * simParam;
	SimulationName * simName;
	SimulationStepsToSimulate * simStepsToSimulate;
	SimulationStepInterval * simStepInterval;
	SimulationNodes * simNodes;
	SimulationTemplate * simTemplate;
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
%token <token> NODE_ACTIVATION_ENUM
%token <token> COLOR
%token <token> NODE_ACTIVATION_MODE_ENUM
%token <token> BOOLEAN
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
%token <token> ID


%token <token> UNKNOWN

/** TODO: Non-terminals. */
%type <program> program
%type <simulation> simulation
%type <simParams> simParams
%type <simParam> simParam
%type <simName> simName
%type <simStepsToSimulate> simStepsToSimulate
%type <simStepInterval> simStepInterval
%type <simNodes> simNodes
%type <vector> vector
%type <simTemplate> simTemplate

/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left ADD SUB
%left MUL DIV

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

program: simulation program																{}
	|	simTemplate program																{}
	|	%empty																			{}

simTemplate: TEMPLATE COLON SIMULATION ID OPEN_PARENTHESIS CLOSE_PARENTHESIS simNodes	{}

simulation: SIMULATION simParams simNodes												{}

simParams: OPEN_PARENTHESIS simParam COMMA simParam COMMA simParam CLOSE_PARENTHESIS	{}
	|	OPEN_PARENTHESIS simParam COMMA simParam CLOSE_PARENTHESIS						{}
	|	OPEN_PARENTHESIS simParam CLOSE_PARENTHESIS										{}
	|	OPEN_PARENTHESIS CLOSE_PARENTHESIS												{}

simParam: simName																		{}
	|	simStepsToSimulate																{}
	|	simStepInterval																	{}

simName: SIMULATION_NAME EQUALS STRING[name]											{}

simStepsToSimulate: STEPS_TO_SIMULATE EQUALS INTEGER[steps]								{}

simStepInterval: STEP_INTERVAL EQUALS INTEGER[interval]									{}

simNodes: OPEN_BRACKET CLOSE_BRACKET													{}

vector: OPEN_PARENTHESIS INTEGER[x] COMMA INTEGER[y] CLOSE_PARENTHESIS					{$$ = VectorSemanticAction($x, $y);}
%%
