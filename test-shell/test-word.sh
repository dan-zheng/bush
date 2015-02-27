#!/bin/bash

source utils.sh
describe "Words and special characters"

grade5=0
grade5max=2
./test-word-1.sh && grade5=`expr $grade5 "+" 1`
./test-word-2.sh && grade5=`expr $grade5 "+" 1`
echo ""
suit $grade5 $grade5max
echo ""
