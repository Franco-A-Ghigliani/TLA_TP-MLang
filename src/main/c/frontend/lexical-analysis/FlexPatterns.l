%{

#include "FlexActions.h"

%}

/**
 * Enable start condition manipulation functions.
 */
%option stack

/**
 * Flex contexts (a.k.a. start conditions).
 *
 * @see https://westes.github.io/flex/manual/Start-Conditions.html
 */
%x MULTILINE_COMMENT
%x SIMULATION
%x SIMULATION_PARAMS
%x SIMULATION_NODES

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */

%%

"/*"								{ BEGIN(MULTILINE_COMMENT); BeginMultilineCommentLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>"*/"				{ EndMultilineCommentLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
<MULTILINE_COMMENT>[^*]+			{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>.				{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }

"Simulation"                        { BEGIN(SIMULATION); BeginSimulationLexemeAction(createLexicalAnalyzerContext()); }
<SIMULATION_NODES> "}"              { EndSimulationLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }

<SIMULATION> "("                    { BEGIN(SIMULATION_PARAMS); BeginSimulationParamsLexemeAction(createLexicalAnalyzerContext()); }
<SIMULATION_PARAMS> ")"             { EndSimulationParamsLexemeAction(createLexicalAnalyzerContext()); BEGIN(SIMULATION); }

<SIMULATION_PARAMS> [a-zA-Z]+       { return ParamLexemeAction(createLexicalAnalyzerContext()); }
<SIMULATION_PARAMS> "="             { return EqualsLexemeAction(createLexicalAnalyzerContext()); }
<SIMULATION_PARAMS> \"[^"]+\"       { return StringLexemeAction(createLexicalAnalyzerContext()); }
<SIMULATION_PARAMS> ","             { return CommaLexemeAction(createLexicalAnalyzerContext()); }
<SIMULATION_PARAMS> [[:digit:]]+    { return IntegerLexemeAction(createLexicalAnalyzerContext()); }

<SIMULATION> "{"                    { BEGIN(SIMULATION_NODES); BeginSimulationNodesLexemeAction(createLexicalAnalyzerContext()); }

[[:space:]]+						{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
.									{ return UnknownLexemeAction(createLexicalAnalyzerContext()); }

%%

#include "FlexExport.h"