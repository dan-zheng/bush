#!/bin/sh

source utils.sh

grade3=0
grade3max=5
echo "--- Background and Zombie Processes -----------------"
./test-bg-1.sh && grade3=`expr $grade3 "+" 3`
./test-bg-2.sh && grade3=`expr $grade3 "+" 2`
echo ""
suit $grade3 $grade3max
echo ""
