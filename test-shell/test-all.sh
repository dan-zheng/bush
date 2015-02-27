#!/bin/bash

source ./utils.sh

source ./test-io.sh
source ./test-pipe.sh
source ./test-bg.sh
source ./test-env.sh
source ./test-word.sh
source ./test-cd.sh
source ./test-glob.sh
source ./test-esc.sh
source ./test-ctrlc.sh
source ./test-misc.sh
source ./test-subsh.sh
source ./test-tilde.sh
source ./test-bug.sh

total=`expr $grade1 + $grade2 + $grade3 + $grade4 + $grade5 + \
        $grade6 + $grade7 + $grade8 + $grade9 + $grade10 + \
	      $grade11 + $grade12`

totalmax=`expr $grade1max + $grade2max + $grade3max + $grade4max \
            + $grade5max + $grade6max + $grade7max + $grade8max \
            + $grade9max + $grade10max + $grade11max + $grade12max`

function report_row() {
  if [ $2 = $3 ]; then
    pgreenf "✔"
  else
    predf "✘"
  fi


  if [ $2 = $3 ]; then
    pgray "  $1"
  else
    pred "  $1    ($2 / $3)"
  fi
}

echo
echo
report_row "Previously Fixed Issues" $grade_bug $grade_bug_max
echo
echo
report_row "IO Redirection"          $grade1  $grade1max
report_row "Pipes"                   $grade2  $grade2max
report_row "Background and Zombie"   $grade3  $grade3max
report_row "Environment"             $grade4  $grade4max
report_row "Words and Special Chars" $grade5  $grade5max
report_row "cd"                      $grade6  $grade6max
report_row "Wildcarding"             $grade7  $grade7max
report_row "Quotes and Escape Chars" $grade8  $grade8max
report_row "Ctrl-C"                  $grade9  $grade9max
report_row "Robustness"              $grade10 $grade10max
report_row "Subshell"                $grade11 $grade11max
report_row "Tilde Expansion"         $grade12 $grade12max
echo   "----------------------------------------------------"
report_row "Total"                   $total   $totalmax

cleanup
