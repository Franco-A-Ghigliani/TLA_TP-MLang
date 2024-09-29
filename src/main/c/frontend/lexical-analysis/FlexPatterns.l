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
%x SIMULATION_NODES
%x CONNECTION
%x VECTOR
%x FORMULA
%x INSTANTIATION
%x NODE
%x CONSTANT
%x SIMULATION_PARAMS
%x TEMPLATE_INSTANTIATION
%x PARAMS_VALUE

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */

%%

"Simulation"                                { BEGIN(SIMULATION); return BeginSimulationLexemeAction(createLexicalAnalyzerContext()); }
"Template"                                  { ; }
":"                                         { ; }
"Const"


<SIMULATION_NODES>"}"                       { EndSimulationLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
<SIMULATION_NODES>"Template"
<SIMULATION_NODES>":"
<SIMULATION_NODES>"}"
<SIMULATION_NODES>[a-zA-Z]+
<SIMULATION_NODES>"new"

<SIMULATION>"("                             { BEGIN(SIMULATION_PARAMS); BeginSimulationParamsLexemeAction(createLexicalAnalyzerContext()); }
<SIMULATION>"{"

<CONNECTION>"()"
<CONNECTION>[a-zA-Z]+
<CONNECTION>";"
<CONNECTION>"=>"
<CONNECTION>"=#"
<CONNECTION>"."

<PARAMS_VALUE>"("
<PARAMS_VALUE>")"
<PARAMS_VALUE>
<PARAMS_VALUE>

<SIMULATION_PARAMS>")"                      { EndSimulationParamsLexemeAction(createLexicalAnalyzerContext()); BEGIN(SIMULATION); }
<SIMULATION_PARAMS>"name"
<SIMULATION_PARAMS>"stepInterval"
<SIMULATION_PARAMS>"stepsToSimulate"
<SIMULATION_PARAMS>[a-zA-Z]+                { return ParamLexemeAction(createLexicalAnalyzerContext()); }
<SIMULATION_PARAMS>"="                      { BEGIN(PARAMS_VALUE); BeginParamsValueLexemeAction(createLexicalAnalyzerContext()); }

<SIMULATION>"{"                             { BEGIN(SIMULATION_NODES); BeginSimulationNodesLexemeAction(createLexicalAnalyzerContext()); }

<*>"//"[^\n]*								{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<*>[[:space:]]+						        { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<*>.									    { return UnknownLexemeAction(createLexicalAnalyzerContext()); }

%%

#include "FlexExport.h"