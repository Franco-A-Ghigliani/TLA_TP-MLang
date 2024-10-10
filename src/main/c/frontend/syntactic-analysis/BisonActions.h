#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include "AbstractSyntaxTree.h"
#include "SyntacticAnalyzer.h"
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeBisonActionsModule();

/** Shutdown module's internal state. */
void shutdownBisonActionsModule();

/**
 * Bison semantic actions.
 */

Program * wrapperProgramSemanticAction(CompilerState* state, SimulationWrapper* wrapper);

SimulationWrapper* constantWrapperSemanticAction(Constant* const, SimulationWrapper* next);
SimulationWrapper* templateWrapperSemanticAction(SimulationTemplate* template, SimulationWrapper* next);
SimulationWrapper* simulationWrapperSemanticAction(Simulation* sim);
SimulationWrapper* emptyWrapperSemanticAction();

Constant* expressionConstantSemanticAction(char* id, Expression* exp);
Constant* stringConstantSemanticAction(char* id, char* str);

SimulationTemplate* templateSemanticAction(char* id, SimElements* elements);

Simulation* simulationSemanticAction(SimulationParams* params, SimElements* elements);

SimElements* nodeElementsSemanticAction(SimulationNode* node, SimElements* next);
SimElements* nodeTemplateElementsSemanticAction(SimulationNode* node, SimElements* next);
SimElements* intanciationElementsSemanticAction(TemplateInstanciate* inst, SimElements* next);
SimElements* connectionElementsSemanticAction(SimConnection* connection, SimElements* next);
SimElements* emptyElementsSemanticAction();

TemplateInstanciate* nodeInstanciationSemanticAction(char* templateId, char* instanceId, NodeParams* params);
TemplateInstanciate* simInstanciationSemanticAction(char* templateId, char* instanceId);

//---------------------------------------------------------------------------------------
//------------------------------SIM PARAMS-----------------------------------------------
SimulationParams* simParamsSemanticAction(SimulationParam* param1, SimulationParam* param2, SimulationParam* param3);

SimulationParam* nameSimParamSemanticAction(char* name);
SimulationParam* stepsToSimulateSimParamSemanticAction(int steps);
SimulationParam* stepIntervalSimParamSemanticAction(int interval);

//----------------------------------------------------------------------------------------
//-------------------------------------SIM NODE-------------------------------------------
SimulationNode* nodeSemanticAction(char* id,  NodeParams* params, NodeType type);

NodeParams* nodeParamsSemanticAction(NodeParam* param, NodeParams* next);

NodeParam* labelParamSemanticAction(char* label);
NodeParam* postionParamSemanticAction(Vector* vector);
NodeParam* nodeActivationParamSemanticAction(Activation activation);
NodeParam* activationModeParamSemanticAction(ActivationMode mode);
NodeParam* resourceColorParamSemanticAction(Color color);
NodeParam* initialResourcesParamSemanticAction(Expression* exp);
NodeParam* initialResourcesColorParamSemanticAction(Color color);
NodeParam* capacityParamSemanticAction(Expression* exp);
NodeParam* numberDisplayThresholdParamSemanticAction(Expression* exp);
NodeParam* drainOnOverflowParamSemanticAction(boolean val);
NodeParam* randomDistParamSemanticAction(boolean val);
NodeParam* multiconversionParamSemanticAction(boolean val);
NodeParam* queueParamSemanticAction(boolean val);

//--------------------------------------------------------------------------------------------
//---------------------------------------SIM CONNECTION---------------------------------------
SimConnection* connectionSemanticAction(NodeReference* src, NodeReference* dest, Formula* formula, ConnectionType type);

Formula* arithmeticFormulaSemanticAction(Expression* expression, FormulaType type);

Expression* arithmeticExpressionSemanticAction(Expression* left, Expression* right, ExpressionType type);
Expression* factorExpressionSemanticAction(Factor * factor);

Factor* idFactorSemanticAction(char* id);
Factor* integerFactorSemanticAction(int val);

NodeReference* nodeReferenceSemanticAction(char* id, char* nodeId);
	
//----------------------------------------------------------------------------------------
//-------------------------------VECTOR---------------------------------------------------
Vector* vectorSemanticAction(int x, int y);

#endif
