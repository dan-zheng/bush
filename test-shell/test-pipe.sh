#!/bin/bash

source utils.sh
describe "Pipes"

grade2=0
grade2max=15
./test-pipe-1.sh && grade2=`expr $grade2 "+" 4`
./test-pipe-2.sh && grade2=`expr $grade2 "+" 4`
./test-pipe-3.sh && grade2=`expr $grade2 "+" 4`
./test-pipe-4.sh && grade2=`expr $grade2 "+" 3`
echo ""
suit $grade2 $grade2max
echo ""
