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

typedef enum SimParamType SimParamType;


typedef struct Vector Vector;

typedef struct Program Program;
typedef struct Simulation Simulation;
typedef struct SimulationTemplate SimulationTemplate;

typedef struct SimulationParams SimulationParams;
typedef struct SimulationParam SimulationParam;
typedef struct SimulationName SimulationName;
typedef struct SimulationStepsToSimulate SimulationStepsToSimulate;
typedef struct SimulationStepInterval SimulationStepInterval;

typedef struct SimulationNodes SimulationNodes;
typedef struct Node Node;

/**
 * Node types for the Abstract Syntax Tree (AST).
 */

enum Activation {
	AUTOMATIC,
	ON_START,
	PASSIVE,
	INTERACTIVE
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

enum SimParamType {
	NAME,
	STEPS,
	STEP_INTERVAL
};


struct Vector {
	int x;
	int y;
};

struct Node {
	
};

struct SimulationName {
	char* name;
};


struct SimulationStepsToSimulate {
	int steps;
};


struct SimulationStepInterval {
	int interval;
};

struct SimulationParam {
	union {
		SimulationName * name;
		SimulationStepsToSimulate * steps;
		SimulationStepInterval * stepInterval;
	};
	SimParamType type;
};

struct SimulationParams {
	SimulationParam* name;
	SimulationParam* steps;
	SimulationParam* stepInterval;
};

struct SimulationNodes {
	Node* firstNode;
};

struct Simulation {
	SimulationParams * params;
	SimulationNodes * nodes;
	Simulation * next;
};

struct SimulationTemplate {
	SimulationNodes * nodes;
};


struct Program {
	Simulation* firstSimulation;
};

/**
 * Node recursive destructors.
 */
void releaseProgram(Program * program);

#endif
