#!/bin/bash

THREADS=(2 4 8 16 32)
FILES=("philosophers" "producer_consumer" "reader_writer")

mkdir -p "analyses"
echo "Début"
for programme in "${FILES[@]}";
    do
    echo "thread,essai n°,time" > "analyses/${programme}.csv"
    echo "fichiers créés"
    gcc -Wall -o "$programme" "$programme".c -lpthread
    for thread in "${THREADS[@]}"; do
	if [ "$programme" == "philosophers" ];
	then
	for (( i=1; i<=5; i++)); do
	    starttime=$(date +%s.%N)
	    ./"$programme" "$thread"
	    endtime=$(date +%s.%N)
	    totaltime=$(echo "$endtime - $starttime" | bc -l)
	    echo "$thread,$i,$totaltime" >> "analyses/${programme}.csv"
	done
	else
	for (( i=1; i<=5; i++)); do
	    divided=$((thread/2))
	    starttime=$(date +%s.%N)
	    ./"$programme" "$divided" "$divided"
	    endtime=$(date +%s.%N)
	    totaltime=$(echo "$endtime - $starttime" | bc -l)
	    echo "$thread,$i,$totaltime" >> "analyses/${programme}.csv"
	done
	fi
    done
done
echo "Fini"
