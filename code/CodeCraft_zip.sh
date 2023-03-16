#!/bin/bash

SCRIPT=$(readlink -f "$0")
BASEDIR=$(dirname "$SCRIPT")
cd $BASEDIR

if [ ! -f CMakeLists.txt ]
then
    echo "ERROR: $BASEDIR is not a valid CMake file of SDK_C++ for CodeCraft-2023."
    echo "  Please run this script in a regular directory of SDK_C++."
    exit -1
fi

yesterday=`date +%Y-%m-%d--%H:%M:%S`

echo "Current date: $yesterday"
zip -9 -r $yesterday.zip *.cpp *.cc *.c *.hpp *.h CMakeLists.txt