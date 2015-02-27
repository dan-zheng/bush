#!/bin/bash

source ./utils.sh
describe "Quotes and Escape characters"

grade8=0
grade8max=5
./test-esc-1.sh && grade8=`expr $grade8 "+" 1`
./test-esc-2.sh && grade8=`expr $grade8 "+" 1`
./test-esc-3.sh && grade8=`expr $grade8 "+" 1`
./test-esc-4.sh && grade8=`expr $grade8 "+" 2`
echo ""
suit $grade8 $grade8max
echo ""
