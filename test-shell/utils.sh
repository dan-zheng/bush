#!/bin/bash

if [ -z "$UTILS_SH_" ]
then

    trap "exit 1" TERM
    export TOP_PID=$$

    UTILS_SH_=yes

    SHELL=../shell

    CSH_IN=cshin.tmp.sh
    CSH_OUT=cshout.tmp.txt
    CSH_ERR=csherr.tmp.txt
    BUSH_IN=bushin.tmp.sh
    BUSH_OUT=bushout.tmp.txt
    BUSH_ERR=busherr.tmp.txt


    # Mocha Style
    function it {
      testname="$1"
    }
    function describe {
      echo -e "\e[4;37m$1\e[0m"
    }
    function panic {
      errmessage = $errmessage"\n"$1
    }

    function check {
      if [ $1 -ne 0 ]
      then
        fail $2
      fi
    }

    function succeed {
      pgreen "✔\c"
      pgray "  $testname"
    }
    function fail    {
      pred "✘\c"
      echo "  $testname"
      if [ ! -z $errmessage ]; then
        it $errmessage
      fi
      kill -s TERM $TOP_PID
    }
    function suit {
      if [ $1 = $2 ]; then
        pgreen "✔  $1 / $2 pts"
      else
        pred "✘  $1 / $2 pts"
      fi
    }

    function pgray {
      echo -e "\e[1;30m$1\e[0m"
    }
    function pgreen {
      echo -e "\e[1;32m$1\e[0m"
    }
    function pred {
      echo -e "\e[1;31m$1\e[0m"
    }

    function cleanup {
      rm -f *.tmp.*
    }

fi
