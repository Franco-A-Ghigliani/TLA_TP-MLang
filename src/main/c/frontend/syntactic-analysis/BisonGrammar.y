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

	/** Non-terminals. */

	Constant * constant;
	Expression * expression;
	Factor * factor;
	Program * program;
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

//DATA TYPES
%token <integer> INTEGER
%token <string> STRING

//OPERATORS
%token <token> EQUALS
%token <token> COMMA
%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS
%token <token> TEMPLATE
%token <token> SIM
%token <token> COLON
%token <token> OPEN_BRACKET
%token <token> CLOSE_BRACKET
%token <token> NEW
%token <token> SEMI_COLON
%token <token> RESOURCE_CONNECT
%token <token> STATE_CONNECT
%token <token> PERIOD
%token <token> LESS_THAN
%token <token> GREATER_THAN
%token <token> PERCENTAGE
%token <token> CONST
%token <token> UNKNOWN

//
%token <string> PARAM
%token <string> ID

//TOKENS PARA SIM_PARAMS
%token <token> SIMULATION_NAME
%token <token> STEP_INTERVAL
%token <token> STEPS_TO_SIMULATE

//TOKENS PARA NODE_TYPES
%token <token> SOURCE
%token <token> DRAIN
%token <token> POOL
%token <token> GATE

//TOKENS PARA NODE_PARAMS
%token <token> NODE_LABEL
%token <token> NODE_POSITION
%token <token> NODE_ACTIVATION
%token <token> RESOURCE_COLOR
%token <token> NODE_ACTIVATION_MODE



/** Non-terminals. */
%type <constant> constant
%type <expression> expression
%type <factor> factor
%type <program> program

/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left ADD SUB
%left MUL DIV

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

program: expression													{ $$ = ExpressionProgramSemanticAction(currentCompilerState(), $1); }
	;

expression: expression[left] ADD expression[right]					{ $$ = ArithmeticExpressionSemanticAction($left, $right, ADDITION); }
	| expression[left] DIV expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, DIVISION); }
	| expression[left] MUL expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, MULTIPLICATION); }
	| expression[left] SUB expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, SUBTRACTION); }
	| factor														{ $$ = FactorExpressionSemanticAction($1); }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS				{ $$ = ExpressionFactorSemanticAction($2); }
	| constant														{ $$ = ConstantFactorSemanticAction($1); }
	;

constant: INTEGER													{ $$ = IntegerConstantSemanticAction($1); }
	;

%%
