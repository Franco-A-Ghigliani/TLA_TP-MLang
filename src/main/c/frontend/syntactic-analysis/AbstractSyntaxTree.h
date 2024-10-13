#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include "../../shared/Logger.h"
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeAbstractSyntaxTreeModule();

/** Shutdown module's internal state. */
void shutdownAbstractSyntaxTreeModule();

/**
 * This typedefs allows self-referencing types.
 */

typedef enum Activation Activation;
typedef enum ActivationMode ActivationMode;
typedef enum Color Color;
typedef enum ValueType ValueType;
typedef enum SimWrapperType SimWrapperType;
typedef enum SimParamType SimParamType;
typedef enum NodeType NodeType;
typedef enum NodeParamType NodeParamType;
typedef enum SimElementsType SimElementsType;
typedef enum TemplateType TemplateType;
typedef enum ConnectionType ConnectionType;
typedef enum FormulaType FormulaType;
typedef enum ExpressionType ExpressionType;
typedef enum FactorType FactorType;


typedef struct Vector Vector;
typedef struct Program Program;
typedef struct Simulation Simulation;
typedef struct SimulationTemplate SimulationTemplate;
typedef struct TemplateInstanciate TemplateInstanciate;
typedef struct SimulationParams SimulationParams;
typedef struct SimulationParam SimulationParam;
typedef struct SimulationWrapper SimulationWrapper;
typedef struct Constant Constant;
typedef struct SimElements SimElements;
typedef struct SimConnection SimConnection;
typedef struct NodeParams NodeParams;
typedef struct NodeParam NodeParam;
typedef struct SimulationNode SimulationNode;
typedef struct NodeReference NodeReference;
typedef struct Formula Formula;
typedef struct Expression Expression;
typedef struct Factor Factor;
/**
 * Node types for the Abstract Syntax Tree (AST).
 */

enum Activation {
	AUTOMATIC,
	ON_START,
	PASSIVE,
	INTERACTIVE
};

enum ValueType {
	VALUE_STRING,
	VALUE_EXPRESSION
};

enum NodeParamType{
	NODE_LABEL,
	NODE_POSITION,
	GATE_RANDOM_DISTRIBUTION,
	NODE_ACTIVATION,
	NODE_ACTIVATION_MODE,
	POOL_INITIAL_RESOURCES,
	NODE_RESOURCE_COLOR,
	POOL_INITIAL_RESOURCES_COLOR,
	POOL_CAPACITY,
	POOL_NUMBER_DISPLAY_THRESHOLD,
	POOL_DRAIN_ON_OVERFLOW,
	CONVERTER_MULTICONVERSION,
	DELAY_QUEUE
};


enum ConnectionType {
	RESOURCE,
	STATE
};

enum FormulaType {
	PERCENTAGE,
	GREATER_THAN,
	LESS_THAN,
	FORMULA_EXPRESSION
};

enum ExpressionType {
	ADDITION,
	DIVISION,
	FACTOR,
	MULTIPLICATION,
	SUBTRACTION,
};

enum ActivationMode {
	PULL_ANY,
	PULL_ALL,
	PUSH_ANY,
	PUSH_ALL
};


enum Color {
	RED,
	BLACK,
	BLUE,
	ORANGE,
	GREEN
};

enum SimWrapperType{
	CONSTANT,
	SIMULATION_TEMPLATE,
	SIMULATION,
	EMPTY_PROGRAM
};

enum NodeType{
	SOURCE,
	POOL,
	GATE,
	DELAY,
	CONVERTER,
	DRAIN,
	END_CONDITION
};

enum SimParamType {
	NAME,
	STEPS,
	STEP_INTERVAL
};

enum SimElementsType{
	CONNECTION,
	NODE,
	NODE_TEMPLATE,
	TEMPLATE_INSTANCIATION,
	EMPTY
};

enum TemplateType{
	SIMULATION_INSTANCE,
	NODE_INSTANCE
};

enum FactorType{
	FACTOR_STRING,
	INTEGER
};

//---------------------------------------PROGRAM--------------------------------------
struct Program {
	SimulationWrapper * simulationWrapper;
};

struct SimulationWrapper {
	union {
		Constant * constant;
		SimulationTemplate * simulationTemplate;
		Simulation * simulation;
	};
	SimWrapperType type;
	SimulationWrapper * nextSimulationWrapper;
};

struct Constant {
	union {
		char * string;
		Expression * expression;
	};
	char * constantName;
	ValueType type;
};

struct SimulationTemplate {
	char * name;
	SimElements * simElements;
};

struct Simulation {
	SimulationParams * params;
	SimElements * simElements;
};

struct SimElements {
	union {
		SimConnection * connection; 
		SimulationNode * node;
		TemplateInstanciate * templateInst;
	};
	SimElementsType type;
	SimElements * next;
};

struct TemplateInstanciate
{
	NodeParams * nodeParams;
	TemplateType type;
	char * name;
	char * templateReference;
};


//------------------------------------SIM NODE-----------------------------------
struct SimulationNode {
	NodeParams * nodeParams;
	boolean isTemplate;
	char* id;
	NodeType type;
};

struct NodeParams {
	NodeParam * nodeParam;
	NodeParams * nextParams;
};

struct NodeParam {
	union {
		Activation activation;
		ActivationMode activationMode;
		char * string;
		Expression * expression;
		Vector * vector;
		Color color;
		boolean boolean;
	};
	NodeParamType type;
};

//----------------------------------------SIM CONNECTION--------------------------------
struct SimConnection {
	NodeReference * from;
	NodeReference * to;
	Formula * formula;
	ConnectionType type;
};

struct NodeReference {
	char * reference;
	NodeReference * next;
};

struct Formula {
	Expression * expression;
	FormulaType type;
};

struct Expression {
	union {
		Factor * factor;
		struct {
			Expression * leftExpression;
			Expression * rightExpression;
		};
		Expression * expression;
	};
	ExpressionType type;
};

struct Factor {
	union {
		int value;
		char * id;
	};
	FactorType type;
};

//------------------------------------------------SIM PARAMS----------------------------
struct SimulationParams {
	SimulationParam* params[3];
};

struct SimulationParam {
	union {
		char * string;
		int value;
	};
	SimParamType type;
};


//------------------------------------------VECTOR----------------------------------------
struct Vector {
	int x;
	int y;
};

/**
 * Node recursive destructors.
 */
void releaseProgram(Program * program);
void releaseSimulationWrapper(SimulationWrapper * wrapper);
void releaseConstant(Constant* constant);
void releaseSimulationTemplate(SimulationTemplate* template);
void releaseSimulation(Simulation* sim);
void releaseSimElements(SimElements* elems);
void releaseTemplateInstanciate(TemplateInstanciate* instance);
void releaseSimulationParams(SimulationParams* params);
void releaseSimulationParam(SimulationParam* param);
void releaseSimulationNode(SimulationNode* node);
void releaseNodeParams(NodeParams* params);
void releaseNodeParam(NodeParam* param);
void releaseSimConnection(SimConnection* connection);
void releaseFormula(Formula* formula);
void releaseExpression(Expression* expression);
void releaseFactor(Factor* factor);
void releaseNodeReference(NodeReference* ref);
void releaseVector(Vector* vec);

#endif
