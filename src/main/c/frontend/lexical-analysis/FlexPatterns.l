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

<CONNECTION>"=>"                                                                    {}
<CONNECTION>"=#"                                                                    {}
<CONNECTION>"."                                                                     {}

<FORMULA>"<"                                                                        {}
<FORMULA>">"                                                                        {}
<FORMULA>"%"                                                                        {}
<FORMULA>"+"                                                                        {}
<FORMULA>"-"                                                                        {}
<FORMULA>"*"                                                                        {}
<FORMULA>"/"                                                                        {}

"Simulation"                                                                        {} 
"const"                                                                             {}
"Template"                                                                          {}
":"                                                                                 {}
<SIM_NODES>"Template"                                                               {}
<SIM_NODES>":"                                                                      {}

<NODE_PARAM_VALUE>"automatic"|"onStart"|"passive"|"interactive"                     {}
<NODE_PARAM_VALUE>"Red"|"Black"|"Green"|"Blue"|"Orange"                             {}
<NODE_PARAM_VALUE>"pull-any"|"pull-all"|"push-any"|"push-all"                       {}
<NODE_PARAM_VALUE>"true"|"false"                                                    {}
<NODE_PARAM_VALUE>";"                                                               {}
<CONNECTION>";"                                                                     {}
<INSTANCIATION>";"                                                                  {}
<CONST>";"                                                                          {}
<NODE_PARAM_VALUE><SIM_PARAM_VALUE>\"[^\n\"]*\"                                     {}

<NODE_PARAM_VALUE><SIM_PARAM_VALUE><VECTOR><FORMULA><CONST>[[:digit:]]+             {}

<NODE_PARAMS>"label"                                                                {}
<NODE_PARAMS>"position"                                                             {}
<NODE_PARAMS>"activation"                                                           {}
<NODE_PARAMS>"resourceColor"                                                        {}
<NODE_PARAMS>"activationMode"                                                       {} 
<NODE_PARAMS>"initialResources"                                                     {}
<NODE_PARAMS>"initialResourcesColor"                                                {}
<NODE_PARAMS>"capacity"                                                             {}
<NODE_PARAMS>"numberDisplayThreshold"                                               {}
<NODE_PARAMS>"drainOnOverflow"                                                      {}
<NODE_PARAMS>"randomDistribution"                                                   {}
<NODE_PARAMS>"multipleConversion"                                                   {}
<NODE_PARAMS>"isQueue"                                                              {}
<NODE_PARAMS><SIM_PARAMS>"="                                                        {}

<SIM_NODES>"Source"                                                                 {}
<SIM_NODES>"Drain"                                                                  {}
<SIM_NODES>"Pool"                                                                   {}
<SIM_NODES>"Gate"                                                                   {}
<SIM_NODES>"Converter"                                                              {}
<SIM_NODES>"Delay"                                                                  {}
<SIM_NODES>"EndCondition"                                                           {}
<SIM_NODES>"new"                                                                    {}
<SIM_NODES>"}"                                                                      {}
<FORMULA>"}"                                                                        {}
<NODE_PARAMS>"}"                                                                    {}

<SIM_PARAM_VALUE>","                                                                {}
<VECTOR>","                                                                         {}

<SIM_PARAMS>"name"                                                                  {}
<SIM_PARAMS>"stepInterval"                                                          {}
<SIM_PARAMS>"stepsToSimulate"                                                       {}
<SIM_PARAMS>")"                                                                     {}
<FORMULA><VECTOR>")"                                                                {}

<SIM>"{"                                                                            {}
<CONNECTION>"{"                                                                     {}
<INSTANCIATION>"{"                                                                  {}
<NODE>"{"                                                                           {}
<SIM>"("                                                                            {}
<NODE_PARAM_VALUE>"("                                                               {}
<FORMULA>"("                                                                        {}
<SIM><NODE><INSTANCIATION><SIM_NODES><CONNECTION><FORMULA>[a-zA-Z_][a-zA-Z_0-9]*    {}

<*>"//"[^\n]*								                                        { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<*>[[:space:]]+						                                                { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<*>.									                                            { return UnknownLexemeAction(createLexicalAnalyzerContext()); }

%%

#include "FlexExport.h"