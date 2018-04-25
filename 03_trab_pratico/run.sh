#!/bin/bash

# ------------------------------------------------------ #
# Verify if the number of parameter is not equal to 1    #
# ------------------------------------------------------ #
if [ $# -ne 1 ]
  then
    echo "ERROR: Invalid number of parameters, use flag -h for information."
    exit 1
fi


# ------------------------------------------------------ #
# Print usage method (-h flag)                           #
# ------------------------------------------------------ #
if [ "$1" == "-h" ]
  then
  	echo
    echo "USAGE: ./run.sh NUMBER_OF_PROCESS"
    echo
    echo "P.S.: Don't forget to 'chmod +x run.sh'"
    echo
    exit 1
fi


# ------------------------------------------------------ #
# The number of process need to be greater than 2        #
# ------------------------------------------------------ #
if [ $1 -lt "2" ]
  then
    echo "ERROR: The minimum number of process is 2."
    exit 1
fi

NUMBER_OF_PROCESS=$1
NUMBER_OF_PROCESS_MINUS_1=$((NUMBER_OF_PROCESS - 1))

PROCESS_BIN="./process"
SOCKET_BASE_PORT="65000"

for ID in `seq 0 $NUMBER_OF_PROCESS_MINUS_1`
do
	PORT=$(($SOCKET_BASE_PORT + $ID))
	PROCESS_SPAWN="gnome-terminal -x bash -c \"$PROCESS_BIN       \
											   $ID                \
											   $PORT              \
											   $NUMBER_OF_PROCESS \
											   \""
											   # ;bash\""
	echo $ID
	echo $PORT
	echo $NUMBER_OF_PROCESS
	eval $PROCESS_SPAWN
done