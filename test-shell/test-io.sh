#!/bin/bash

source utils.sh
describe "IO Redirection"

grade1=0
grade1max=15
./test-io-1.sh && grade1=`expr $grade1 "+" 3`
./test-io-2.sh && grade1=`expr $grade1 "+" 3`
./test-io-3.sh && grade1=`expr $grade1 "+" 3`
./test-io-4.sh && grade1=`expr $grade1 "+" 2`
./test-io-5.sh && grade1=`expr $grade1 "+" 2`
./test-io-6.sh && grade1=`expr $grade1 "+" 2`
echo ""
suit $grade1 $grade1max
echo ""
