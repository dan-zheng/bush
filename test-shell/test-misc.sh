#!/bin/bash

source ./utils.sh
describe "Robustness"

grade10=0
grade10max=10
./test-misc-1.sh && grade10=`expr $grade10 "+" 10`
echo ""
suit $grade10 $grade10max
echo ""
