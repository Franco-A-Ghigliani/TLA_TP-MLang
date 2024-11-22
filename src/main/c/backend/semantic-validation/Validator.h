#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "../symbol-table/SymbolTableADT.h"
#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include <stdarg.h>
#include <stdio.h>


/** Initialize module's internal state. */
void initializeValidatorModule();

/** Shutdown module's internal state. */
void shutdownValidatorModule();

/**
 * Validates semantics of the input using the current compiler state.
 */
boolean validate(CompilerState * compilerState);

#endif