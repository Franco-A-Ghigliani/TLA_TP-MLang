#! /bin/bash

set -euxo pipefail

BASE_PATH="$(dirname "$0")/../.."

# Check if the build directory exists before removing
if [ -d "$BASE_PATH/build" ]; then
    rm --force --recursive "$BASE_PATH/build"
else
    echo "Directory $BASE_PATH/build does not exist"
fi

# Check if FlexScanner.c exists before removing
if [ -f "$BASE_PATH/src/main/c/frontend/lexical-analysis/FlexScanner.c" ]; then
    rm --force "$BASE_PATH/src/main/c/frontend/lexical-analysis/FlexScanner.c"
else
    echo "File $BASE_PATH/src/main/c/frontend/lexical-analysis/FlexScanner.c does not exist"
fi

# Check if BisonParser.c exists before removing
if [ -f "$BASE_PATH/src/main/c/frontend/syntactic-analysis/BisonParser.c" ]; then
    rm --force "$BASE_PATH/src/main/c/frontend/syntactic-analysis/BisonParser.c"
else
    echo "File $BASE_PATH/src/main/c/frontend/syntactic-analysis/BisonParser.c does not exist"
fi

# Check if BisonParser.h exists before removing
if [ -f "$BASE_PATH/src/main/c/frontend/syntactic-analysis/BisonParser.h" ]; then
    rm --force "$BASE_PATH/src/main/c/frontend/syntactic-analysis/BisonParser.h"
else
    echo "File $BASE_PATH/src/main/c/frontend/syntactic-analysis/BisonParser.h does not exist"
fi

cd $BASE_PATH

cmake -S . -B build
cd build
make
cd ..

echo "All done."
