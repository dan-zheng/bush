#!/bin/bash

if [ -z "$UTILS_SH_" ]
then

    trap "exit 1" TERM
    export TOP_PID=$$

    UTILS_SH_=yes

    SHELL=../shell
    
    CSH_IN=cshin.tmp.sh
    CSH_OUT=cshout.tmp.txt
    BUSH_IN=bushin.tmp.sh
    BUSH_OUT=bushout.tmp.txt

    function check {
      if [ $1 -ne 0 ]
      then
        fail
      fi
    }

    function succeed {
      pgreen "✔  OK"
    }
    function fail    {
      pred "✘  FAIL"
      kill -s TERM $TOP_PID
    }

    function pgray {
      echo "\033[1;30m$1\033[0m"
    }

    function pgreen {
      echo "\033[1;32m$1\033[0m"
    }

    function pred {
      echo "\033[1;31m$1\033[0m"
    }

    function suit {
      if [ $1 = $2 ]; then
        pgreen "✔  $1 / $1 pts"
      else
        pred "✘  $1 / $2 pts"
      fi
    }

fi
