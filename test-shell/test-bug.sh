#!/bin/bash

source utils.sh
describe "Previously Encountered Issues"

grade_bug=0
grade_bug_max=1
./test-bug-1.sh && grade_bug=`expr $grade_bug "+" 1`

echo ""
suit $grade_bug $grade_bug_max
echo ""
