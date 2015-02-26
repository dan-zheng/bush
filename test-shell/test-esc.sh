#!/bin/sh

source ./utils.sh

grade8=0
grade8max=5
echo "--- Quotes and Escape characters --------------------"
./test-esc-1.sh && grade8=`expr $grade8 "+" 1`
./test-esc-2.sh && grade8=`expr $grade8 "+" 1`
./test-esc-3.sh && grade8=`expr $grade8 "+" 1`
./test-esc-4.sh && grade8=`expr $grade8 "+" 2`
echo ""
suit $grade8 $grade8max
echo ""
