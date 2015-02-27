#!/bin/bash

source utils.sh
describe "Ctrl-C"

grade9=0
grade9max=5
./test-ctrlc-1.sh && grade9=`expr $grade9 "+" 5`
echo ""
suit $grade9 $grade9max
echo ""
