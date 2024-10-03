#ifndef FLEX_ACTIONS_HEADER
#define FLEX_ACTIONS_HEADER

#include "../../shared/Environment.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include "../../shared/Type.h"
#include "../syntactic-analysis/AbstractSyntaxTree.h"
#include "../syntactic-analysis/BisonParser.h"
#include "LexicalAnalyzerContext.h"
#include <stdio.h>
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeFlexActionsModule();

/** Shutdown module's internal state. */
void shutdownFlexActionsModule();

/**
 * Flex lexeme processing actions.
 */

Token ResourceConnectionLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);
Token StateConnectionLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);
Token PeriodLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);

Token ArithmeticLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, Token token);

Token SimulationLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);
Token ConstantLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);
Token TemplateLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);
Token ColonLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);

Token ActivationEnumLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, Activation activation);
Token ActivationModeEnumLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, ActivationMode activationMode);
Token ColorLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, Color color);
Token BooleanLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, boolean bool);
Token StringLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);
Token IntegerLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);

Token SemiColonLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);

Token NodeParamLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, Token token);
Token EqualsLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);

Token NodeLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, Token token);
Token NewLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);

Token CommaLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);

Token SimParamLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext, Token token);

Token OpenBracketLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);
Token CloseBracketLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);
Token OpenParenthesisLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);
Token CloseParenthesisLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);

Token IdLexemeAction(LexicalAnalyzerContext* lexicalAnalyzerContext);

void IgnoredLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token UnknownLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

#endif
