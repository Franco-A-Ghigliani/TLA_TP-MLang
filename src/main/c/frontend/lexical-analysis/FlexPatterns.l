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
%x CONST
%x SIM
%x SIM_PARAMS
%x SIM_PARAM_VALUE
%x SIM_NODES
%x NODE
%x NODE_PARAMS
%x NODE_PARAM_VALUE
%x INSTANCIATION
%x CONNECTION
%x FORMULA
%x VECTOR

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */

%%

"Simulation"                                { BEGIN(SIMULATION); }


<*>"//"[^\n]*								{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<*>[[:space:]]+						        { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<*>.									    { return UnknownLexemeAction(createLexicalAnalyzerContext()); }

%%

#include "FlexExport.h"