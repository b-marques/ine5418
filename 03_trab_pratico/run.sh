#!/bin/bash

# ------------------------------------------------------ #
# Print usage method (-h flag)                           #
# ------------------------------------------------------ #
if [ "$1" == "--help" ]
  then
    echo
    echo "USAGE: ./run.sh <NUMBER_OF_PROCESS> <MUTUAL_EXCLUSION_POLICY>"
    echo "  |"                         
    echo "  |-NUMBER_OF_PROCESS: minimum 2"                         
    echo "  |"                         
    echo "  |-MUTUAL_EXCLUSION_POLICY: --token-ring"                         
    echo "                             --server"                         
    echo "                             --multicast"                         
    echo                          
    echo "P.S.: Don't forget to 'chmod +x run.sh'"
    echo
    exit 1
fi

# ------------------------------------------------------ #
# Verify if the number of parameter is not equal to 1    #
# ------------------------------------------------------ #
if [ $# -ne 2 ]
  then
    echo
    echo "ERROR: Invalid number of parameters, use flag --help for information."
    echo
    exit 1
fi

# ------------------------------------------------------ #
# The number of process need to be greater than 2        #
# ------------------------------------------------------ #
if [ $1 -lt "2" ]
  then
    echo
    echo "ERROR: The minimum number of process is 2."
    echo
    exit 1
fi

# ------------------------------------------------------ #
# Check for policy requested by the user                 #
# ------------------------------------------------------ #
if [[ "$2" == "--token-ring" || ("$2" == "--server" || "$2" == "--multicast") ]]
  then
    MUTUAL_EXCLUSION_POLICY=$2
  else
    echo
    echo "ERROR: Something went wrong with requested policy, use './run.sh --help' for information." 
    echo
    exit 1
fi



NUMBER_OF_PROCESS=$1
NUMBER_OF_PROCESS_MINUS_1=$((NUMBER_OF_PROCESS - 1))

PROCESS_BIN="./process"

rm -f "mutex.lock"

for ID in `seq 0 $NUMBER_OF_PROCESS_MINUS_1`
do
    X_COORD=$(((ID*355) + 80))
    PROCESS_SPAWN="gnome-terminal --geometry=37x20+$X_COORD+100 -x bash -c     \
                                       \"$PROCESS_BIN             \
                                         $ID                      \
                                         $NUMBER_OF_PROCESS       \
                                         $MUTUAL_EXCLUSION_POLICY \
                                         ;bash\""
                                       #\""
    eval $PROCESS_SPAWN
done
