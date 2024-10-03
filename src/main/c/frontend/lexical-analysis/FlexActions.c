#include "FlexActions.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;
static boolean _logIgnoredLexemes = true;

void initializeFlexActionsModule() {
	_logIgnoredLexemes = getBooleanOrDefault("LOG_IGNORED_LEXEMES", _logIgnoredLexemes);
	_logger = createLogger("FlexActions");
}

void shutdownFlexActionsModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/* PRIVATE FUNCTIONS */

static void _logLexicalAnalyzerContext(const char * functionName, LexicalAnalyzerContext * lexicalAnalyzerContext);

/**
 * Logs a lexical-analyzer context in DEBUGGING level.
 */
static void _logLexicalAnalyzerContext(const char * functionName, LexicalAnalyzerContext * lexicalAnalyzerContext) {
	char * escapedLexeme = escape(lexicalAnalyzerContext->lexeme);
	logDebugging(_logger, "%s: %s (context = %d, length = %d, line = %d)",
		functionName,
		escapedLexeme,
		lexicalAnalyzerContext->currentContext,
		lexicalAnalyzerContext->length,
		lexicalAnalyzerContext->line);
	free(escapedLexeme);
}

/* PUBLIC FUNCTIONS */

Token ResourceConnectionLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext){
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	return RESOURCE_CONNECT;
}

Token StateConnectionLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext){
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	return STATE_CONNECT;
}

Token PeriodLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext){
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	return PERIOD;
}

Token ArithmeticLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, Token token){
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->token = token;
	return token;
}

Token SimulationLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext){
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	return SIMULATION;
}

Token ConstantLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext){
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	return CONSTANT_KEY;
}

Token TemplateLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext){
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	return TEMPLATE;
}

Token ColonLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext){
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	return COLON;
}

Token ActivationEnumLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, Activation activation){
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->activation = activation;
	return NODE_ACTIVATION_ENUM;
}

Token ActivationModeEnumLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, ActivationMode activationMode){
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->activationMode = activationMode;
	return NODE_ACTIVATION_MODE_ENUM;
}

Token ColorLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, Color color){
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->color = color;
	return COLOR;
}

Token BooleanLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, boolean bool){
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->bool = bool;
	return BOOLEAN;
}

Token StringLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext){
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->string = lexicalAnalyzerContext->lexeme;
	return STRING;
}

Token IntegerLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext){
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->integer = atoi(lexicalAnalyzerContext->lexeme);
	return INTEGER;
}

Token SemiColonLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext){
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	return SEMI_COLON;
}

Token NodeParamLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, Token token){
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->token = token;
	return token;
}

Token EqualsLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext){
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	return EQUALS;
}

Token NodeLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, Token token){
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->token = token;
	return token;
}

Token NewLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext){
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	return NEW;
}

Token CommaLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext){
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	return COMMA;
}

Token SimParamLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, Token token){
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->token = token;
	return token;
}

Token OpenBracketLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext){
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	return OPEN_BRACKET;
}

Token CloseBracketLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext){
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	return CLOSE_BRACKET;
}

Token OpenParenthesisLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext){
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	return OPEN_PARENTHESIS;
}

Token CloseParenthesisLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext){
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	return CLOSE_PARENTHESIS;
}

Token IdLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext){
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->string = lexicalAnalyzerContext->lexeme;
	return ID;
}


void IgnoredLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
}

Token UnknownLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	return UNKNOWN;
}
