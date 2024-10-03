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

<CONNECTION>"=>"                                                                    {return ResourceConnectionLexemeAction(createLexicalAnalyzerContext());}
<CONNECTION>"=#"                                                                    {return StateConnectionLexemeAction(createLexicalAnalyzerContext());}
<CONNECTION>"."                                                                     {return PeriodLexemeAction(createLexicalAnalyzerContext());}

<FORMULA>"<"                                                                        {return ArithmeticLexemeAction(createLexicalAnalyzerContext(), LESS_THAN);}
<FORMULA>">"                                                                        {return ArithmeticLexemeAction(createLexicalAnalyzerContext(), GREATER_THAN);}
<FORMULA>"%"                                                                        {return ArithmeticLexemeAction(createLexicalAnalyzerContext(), PERCENTAGE);}
<FORMULA>"+"                                                                        {return ArithmeticLexemeAction(createLexicalAnalyzerContext(), ADD);}
<FORMULA>"-"                                                                        {return ArithmeticLexemeAction(createLexicalAnalyzerContext(), SUBSTRACT);}
<FORMULA>"*"                                                                        {return ArithmeticLexemeAction(createLexicalAnalyzerContext(), MULTIPLY);}
<FORMULA>"/"                                                                        {return ArithmeticLexemeAction(createLexicalAnalyzerContext(), DIVIDE);}

"Simulation"                                                                        {return SimulationLexemeAction(createLexicalAnalyzerContext());} 
"const"                                                                             {Token token = ConstantLexemeAction(createLexicalAnalyzerContext()); BEGIN(CONST); return token;}
"Template"                                                                          {return TemplateLexemeAction(createLexicalAnalyzerContext());}
":"                                                                                 {return ColonLexemeAction(createLexicalAnalyzerContext());}
<SIM_NODES>"Template"                                                               {return TemplateLexemeAction(createLexicalAnalyzerContext());}
<SIM_NODES>":"                                                                      {return ColonLexemeAction(createLexicalAnalyzerContext());}

<NODE_PARAM_VALUE>"automatic"                                                       {return ActivationEnumLexemeAction(createLexicalAnalyzerContext(), AUTOMATIC);}
<NODE_PARAM_VALUE>"onStart"                                                         {return ActivationEnumLexemeAction(createLexicalAnalyzerContext(), ON_START);}
<NODE_PARAM_VALUE>"passive"                                                         {return ActivationEnumLexemeAction(createLexicalAnalyzerContext(), PASSIVE);}
<NODE_PARAM_VALUE>"interactive"                                                     {return ActivationEnumLexemeAction(createLexicalAnalyzerContext(), INTERACTIVE);}

<NODE_PARAM_VALUE>"Red"                                                             {return ColorLexemeAction(createLexicalAnalyzerContext(), RED);}
<NODE_PARAM_VALUE>"Black"                                                           {return ColorLexemeAction(createLexicalAnalyzerContext(), BLACK);}
<NODE_PARAM_VALUE>"Green"                                                           {return ColorLexemeAction(createLexicalAnalyzerContext(), GREEN);}
<NODE_PARAM_VALUE>"Blue"                                                            {return ColorLexemeAction(createLexicalAnalyzerContext(), BLUE);}
<NODE_PARAM_VALUE>"Orange"                                                          {return ColorLexemeAction(createLexicalAnalyzerContext(), ORANGE);}

<NODE_PARAM_VALUE>"pull-any"                                                        {return ActivationModeEnumLexemeAction(createLexicalAnalyzerContext(), PULL_ANY);}
<NODE_PARAM_VALUE>"pull-all"                                                        {return ActivationModeEnumLexemeAction(createLexicalAnalyzerContext(), PULL_ALL);}
<NODE_PARAM_VALUE>"push-any"                                                        {return ActivationModeEnumLexemeAction(createLexicalAnalyzerContext(), PUSH_ANY);}
<NODE_PARAM_VALUE>"push-all"                                                        {return ActivationModeEnumLexemeAction(createLexicalAnalyzerContext(), PUSH_ALL);}

<NODE_PARAM_VALUE>"true"                                                            {return BooleanLexemeAction(createLexicalAnalyzerContext(), true);}
<NODE_PARAM_VALUE>"false"                                                           {return BooleanLexemeAction(createLexicalAnalyzerContext(), false);}

<NODE_PARAM_VALUE>";"                                                               {Token token = SemiColonLexemeAction(createLexicalAnalyzerContext()); BEGIN(NODE_PARAMS); return token;}
<CONNECTION>";"                                                                     {Token token = SemiColonLexemeAction(createLexicalAnalyzerContext()); BEGIN(SIM_NODES); return token;}
<INSTANCIATION>";"                                                                  {Token token = SemiColonLexemeAction(createLexicalAnalyzerContext()); BEGIN(SIM_NODES); return token;}
<CONST>";"                                                                          {Token token = SemiColonLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); return token;}

<NODE_PARAM_VALUE><SIM_PARAM_VALUE>\"[^\n\"]*\"                                     {return StringLexemeAction(createLexicalAnalyzerContext());}
<NODE_PARAM_VALUE><SIM_PARAM_VALUE><VECTOR><FORMULA><CONST>[[:digit:]]+             {return IntegerLexemeAction(createLexicalAnalyzerContext());}

<NODE_PARAMS>"label"                                                                {return NodeParamLexemeAction(createLexicalAnalyzerContext(), NODE_LABEL);}
<NODE_PARAMS>"position"                                                             {return NodeParamLexemeAction(createLexicalAnalyzerContext(), NODE_POSITION);}
<NODE_PARAMS>"activation"                                                           {return NodeParamLexemeAction(createLexicalAnalyzerContext(), NODE_ACTIVATION);}
<NODE_PARAMS>"resourceColor"                                                        {return NodeParamLexemeAction(createLexicalAnalyzerContext(), NODE_RESOURCE_COLOR);}
<NODE_PARAMS>"activationMode"                                                       {return NodeParamLexemeAction(createLexicalAnalyzerContext(), NODE_ACTIVATION_MODE);} 
<NODE_PARAMS>"initialResources"                                                     {return NodeParamLexemeAction(createLexicalAnalyzerContext(), POOL_INITIAL_RESOURCES);}
<NODE_PARAMS>"initialResourcesColor"                                                {return NodeParamLexemeAction(createLexicalAnalyzerContext(), POOL_INITIAL_RESOURCES_COLOR);}
<NODE_PARAMS>"capacity"                                                             {return NodeParamLexemeAction(createLexicalAnalyzerContext(), POOL_CAPACITY);}
<NODE_PARAMS>"numberDisplayThreshold"                                               {return NodeParamLexemeAction(createLexicalAnalyzerContext(), POOL_NUMBER_DISPLAY_THRESHOLD);}
<NODE_PARAMS>"drainOnOverflow"                                                      {return NodeParamLexemeAction(createLexicalAnalyzerContext(), POOL_DRAIN_ON_OVERFLOW);}
<NODE_PARAMS>"randomDistribution"                                                   {return NodeParamLexemeAction(createLexicalAnalyzerContext(), GATE_RANDOM_DISTRIBUTION);}
<NODE_PARAMS>"multipleConversion"                                                   {return NodeParamLexemeAction(createLexicalAnalyzerContext(), CONVERTER_MULTICONVERSION);}
<NODE_PARAMS>"isQueue"                                                              {return NodeParamLexemeAction(createLexicalAnalyzerContext(), DELAY_QUEUE);}

<NODE_PARAMS>"="                                                                    {Token token = EqualsLexemeAction(createLexicalAnalyzerContext()); BEGIN(NODE_PARAM_VALUE); return token;}
<SIM_PARAMS>"="                                                                     {Token token = EqualsLexemeAction(createLexicalAnalyzerContext()); BEGIN(SIM_PARAM_VALUE); return token;}

<SIM_NODES>"Source"                                                                 {Token token = NodeLexemeAction(createLexicalAnalyzerContext(), SOURCE); BEGIN(NODE); return token; }
<SIM_NODES>"Drain"                                                                  {Token token = NodeLexemeAction(createLexicalAnalyzerContext(), DRAIN); BEGIN(NODE); return token;}
<SIM_NODES>"Pool"                                                                   {Token token = NodeLexemeAction(createLexicalAnalyzerContext(), POOL); BEGIN(NODE); return token;}
<SIM_NODES>"Gate"                                                                   {Token token = NodeLexemeAction(createLexicalAnalyzerContext(), GATE); BEGIN(NODE); return token;}
<SIM_NODES>"Converter"                                                              {Token token = NodeLexemeAction(createLexicalAnalyzerContext(), CONVERTER); BEGIN(NODE); return token;}
<SIM_NODES>"Delay"                                                                  {Token token = NodeLexemeAction(createLexicalAnalyzerContext(), DELAY); BEGIN(NODE); return token;}
<SIM_NODES>"EndCondition"                                                           {Token token = NodeLexemeAction(createLexicalAnalyzerContext(), END_CONDITION); BEGIN(NODE); return token;}
<SIM_NODES>"new"                                                                    {Token token = NewLexemeAction(createLexicalAnalyzerContext()); BEGIN(INSTANCIATION); return token;}
<SIM_NODES>"}"                                                                      {Token token = CloseBracketLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); return token;}
<FORMULA>"}"                                                                        {Token token = CloseBracketLexemeAction(createLexicalAnalyzerContext()); BEGIN(CONNECTION); return token;}
<NODE_PARAMS>"}"                                                                    {Token token = CloseBracketLexemeAction(createLexicalAnalyzerContext()); BEGIN(SIM_NODES); return token;}

<SIM_PARAM_VALUE>","                                                                {Token token = CommaLexemeAction(createLexicalAnalyzerContext()); BEGIN(SIM_PARAMS); return token;}
<VECTOR>","                                                                         {return CommaLexemeAction(createLexicalAnalyzerContext());}

<SIM_PARAMS>"name"                                                                  {return SimParamLexemeAction(createLexicalAnalyzerContext(), SIMULATION_NAME);}
<SIM_PARAMS>"stepInterval"                                                          {return SimParamLexemeAction(createLexicalAnalyzerContext(), STEP_INTERVAL);}
<SIM_PARAMS>"stepsToSimulate"                                                       {return SimParamLexemeAction(createLexicalAnalyzerContext(), STEPS_TO_SIMULATE);}
<SIM_PARAMS>")"                                                                     {Token token = CloseParenthesisLexemeAction(createLexicalAnalyzerContext()); BEGIN(SIMULATION); return token;}
<FORMULA>")"                                                                        {return CloseParenthesisLexemeAction(createLexicalAnalyzerContext());}
<VECTOR>")"                                                                         {Token token = CloseParenthesisLexemeAction(createLexicalAnalyzerContext()); BEGIN(NODE_PARAM_VALUE); return token;}

<SIM>"{"                                                                            {Token token = OpenBracketLexemeAction(createLexicalAnalyzerContext()); BEGIN(SIM_NODES); return token;}
<CONNECTION>"{"                                                                     {Token token = OpenBracketLexemeAction(createLexicalAnalyzerContext()); BEGIN(FORMULA); return token;}
<INSTANCIATION><NODE>"{"                                                            {Token token = OpenBracketLexemeAction(createLexicalAnalyzerContext()); BEGIN(NODE_PARAMS); return token;}
<SIM>"("                                                                            {Token token = OpenParenthesisLexemeAction(createLexicalAnalyzerContext()); BEGIN(SIM_PARAMS); return token;}
<NODE_PARAM_VALUE>"("                                                               {Token token = OpenParenthesisLexemeAction(createLexicalAnalyzerContext()); BEGIN(VECTOR); return token;}
<FORMULA>"("                                                                        {return OpenParenthesisLexemeAction(createLexicalAnalyzerContext()); }
<SIM><NODE><INSTANCIATION><SIM_NODES><CONNECTION><FORMULA>[a-zA-Z_][a-zA-Z_0-9]*    {return IdLexemeAction(createLexicalAnalyzerContext());}

<*>"//"[^\n]*								                                        { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<*>[[:space:]]+						                                                { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<*>.									                                            { return UnknownLexemeAction(createLexicalAnalyzerContext()); }

%%

#include "FlexExport.h"