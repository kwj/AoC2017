#!/bin/sh

if [ $# -eq 0 ]; then
    echo "usage: solve.sh <problem_number>"
    exit 1
fi

exe="./build/src/aoc2017"

if [ ! -x ${exe} ]; then
    echo "There is no exectuion file: $exe"
else
    ${exe} $1 
fi
