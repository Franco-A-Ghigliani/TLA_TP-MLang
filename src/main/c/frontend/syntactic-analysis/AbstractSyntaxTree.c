#include "AbstractSyntaxTree.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeAbstractSyntaxTreeModule() {
	_logger = createLogger("AbstractSyntxTree");
}

void shutdownAbstractSyntaxTreeModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PUBLIC FUNCTIONS */

void releaseProgram(Program * program) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (program != NULL) {
		if(program->simulationWrapper != NULL){
			releaseSimulationWrapper(program->simulationWrapper);
		}
		free(program);
	}
}

void releaseSimulationWrapper(SimulationWrapper * wrapper){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (wrapper != NULL) {
		if(wrapper->constant != NULL){
			releaseConstant(wrapper->constant);
		}
		if(wrapper->simulationTemplate != NULL){
			releaseSimulationTemplate(wrapper->simulationTemplate);
		}
		if(wrapper->simulation != NULL){
			releaseSimulation(wrapper->simulation);
		}
		if(wrapper->nextSimulationWrapper != NULL){
			releaseSimulationWrapper(wrapper->nextSimulationWrapper);
		}
		free(wrapper);
	}
}

void releaseConstant(Constant* constant){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (constant != NULL) {
		if(constant->string != NULL){
			free(constant->string);
		}
		if(constant->expression != NULL){
			releaseExpression(constant->expression);
		}
		if(constant->constantName != NULL){
			free(constant->constantName);
		}
		free(constant);
	}
}

void releaseSimulationTemplate(SimulationTemplate* template){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (template != NULL) {
		if(template->name != NULL){
			free(template->name);
		}
		if (template->simElements != NULL) {
			releaseSimElements(template->simElements);
		}
		free(template);
	}
}

void releaseSimulation(Simulation* sim){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(sim != NULL){
		if(sim->params != NULL){
			releaseSimulationParams(sim->params);
		}
		if(sim->simElements != NULL){
			releaseSimElements(sim->simElements);
		}
		free(sim);
	}
}

void releaseSimElements(SimElements* elems){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (elems != NULL) {
		if(elems->connection != NULL){
			releaseSimConnection(elems->connection);
		}
	
		if(elems->node != NULL){
			releaseSimulationNode(elems->node);
		}
		if(elems->templateInst != NULL){
			releaseTemplateInstanciate(elems->templateInst);
		}
		if(elems->next != NULL){
			releaseSimElements(elems->next);
		}
		free(elems);
	}
}

void releaseTemplateInstanciate(TemplateInstanciate* instance){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(instance != NULL){
		if(instance->nodeParams != NULL){
			releaseNodeParams(instance->nodeParams);
		}
		if(instance->name != NULL){
			free(instance->name);
		}
		if(instance->templateReference != NULL){
			free(instance->templateReference);
		}
		free(instance);
	}

}

void releaseSimulationParams(SimulationParams* params){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(params != NULL){
		for (int i = 0; i < 3; i++)
		{
			if(params->params[i] != NULL){
				releaseSimulationParam(params->params[i]);
			}
		}
		free(params);
	}
}

void releaseSimulationParam(SimulationParam* param){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(param != NULL){
		if(param->string != NULL) {
			free(param->string);
		}
		free(param);
	}
}

void releaseSimulationNode(SimulationNode* node){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(node != NULL){
		if(node->id != NULL) {
			free(node->id);
		}
		if(node->nodeParams != NULL){
			releaseNodeParams(node->nodeParams);
		}
		free(node);
	}
}

void releaseNodeParams(NodeParams* params){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(params != NULL){
		if(params->nodeParam != NULL) {
			releaseNodeParam(params->nodeParam);
		}
		if(params->nextParams != NULL){
			releaseNodeParams(params->nextParams);
		}
		free(params);
	}
}

void releaseNodeParam(NodeParam* param){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(param != NULL){
		if(param->string != NULL) {
			free(param->string);
		}
		if(param->expression != NULL){
			releaseExpression(param->expression);
		}
		if(param->vector != NULL){
			releaseVector(param->vector);
		}
		free(param);
	}
}

void releaseSimConnection(SimConnection* connection){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(connection != NULL){
		if(connection->from != NULL) {
			releaseNodeReference(connection->from);
		}
		if(connection->to != NULL){
			releaseNodeReference(connection->to);
		}
		if(connection->formula != NULL){
			releaseFormula(connection->formula);
		}
		free(connection);
	}
}

void releaseFormula(Formula* formula){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(formula != NULL){
		if(formula->expression != NULL) {
			releaseExpression(formula->expression);
		}
		free(formula);
	}
}

void releaseExpression(Expression* expression){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(expression != NULL){
		if(expression->factor != NULL) {
			releaseFactor(expression->factor);
		}
		if(expression->leftExpression != NULL){
			releaseExpression(expression->leftExpression);
		}
		if(expression->rightExpression != NULL){
			releaseExpression(expression->rightExpression);
		}
		if(expression->expression != NULL){
			releaseExpression(expression->expression);
		}
		free(expression);
	}
}

void releaseFactor(Factor* factor){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(factor != NULL){
		if(factor->id != NULL) {
			free(factor->id);
		}
		if(factor->exp != NULL){
			releaseExpression(factor->exp);
		}
		free(factor);
	}
}

void releaseNodeReference(NodeReference* ref){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(ref != NULL){
		if(ref->reference != NULL) {
			free(ref->reference);
		}
		if(ref->next != NULL){
			releaseNodeReference(ref->next);
		}
		free(ref);
	}
}

void releaseVector(Vector* vec){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(vec != NULL){
		free(vec);
	}
}


