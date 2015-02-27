#!/bin/bash

source utils.sh
describe "File Globbing"

grade7=0
grade7max=14
./test-glob-1.sh && grade7=`expr $grade7 "+" 2`
./test-glob-2.sh && grade7=`expr $grade7 "+" 1`
./test-glob-3.sh && grade7=`expr $grade7 "+" 1`
./test-glob-4.sh && grade7=`expr $grade7 "+" 2`
./test-glob-5.sh && grade7=`expr $grade7 "+" 2`
./test-glob-6.sh && grade7=`expr $grade7 "+" 4`
./test-glob-7.sh && grade7=`expr $grade7 "+" 2`
echo ""
suit $grade7 $grade7max
echo ""
