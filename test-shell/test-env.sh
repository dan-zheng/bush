#!/bin/bash

source utils.sh
describe "Environemt Variables"

grade4=0
grade4max=10
./test-env-1.sh && grade4=`expr $grade4 "+" 1`
./test-env-2.sh && grade4=`expr $grade4 "+" 2`
./test-env-3.sh && grade4=`expr $grade4 "+" 2`
./test-env-4.sh && grade4=`expr $grade4 "+" 2`
./test-env-5.sh && grade4=`expr $grade4 "+" 2`
./test-env-6.sh && grade4=`expr $grade4 "+" 1`
echo ""
suit $grade4 $grade4max
echo ""
