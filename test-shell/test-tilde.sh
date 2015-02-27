#!/bin/bash

source ./utils.sh
describe "Tilde Expansion"

grade12=0
grade12max=4
./test-tilde-1.sh && grade12=`expr $grade12 "+" 4`
echo ""
suit $grade12 $grade12max
echo ""
