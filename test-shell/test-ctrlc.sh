#!/bin/sh

source utils.sh

grade9=0
grade9max=5
echo "--- Ctrl-C ------------------------------------------"
./test-ctrlc-1.sh && grade3=`expr $grade9 "+" 5`
echo ""
suit $grade9 $grade9max
echo ""
