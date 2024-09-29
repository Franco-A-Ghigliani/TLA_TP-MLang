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

void BeginMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void EndMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void IgnoredLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token BeginSimulationLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void EndSimulationLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

void BeginSimulationParamsLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void EndSimulationParamsLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

void BeginParamsValueLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void EndParamsValueLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

void BeginSimulationNodesLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token ParamLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token StringLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token EqualsLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token CommaLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token IntegerLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token UnknownLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

#endif
