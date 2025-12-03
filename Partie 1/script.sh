#!/bin/bash

THREADS=(2 4 8 16 32)
FILES=("philosophers" "producer_consumer" "reader_writer")

mkdir -p "analyses"
for programme in "${FILES[@]}";
    do
    echo "thread,essai n°,time" > "analyses/${programme}.csv"
    gcc -Wall -o "$programme" "$programme".c -lpthread
    for thread in "${THREADS[@]}"; do # '@' permet d'itérer sur chaque élément dans THREADS
	if [ "$programme" == "philosophers" ];
	then
	for (( i=1; i<=5; i++)); do
	    starttime=$(date +%s.%N) #%s: c'est les secondes depuis 01/01/1970 et %N: nanosecondes pour plus de précision
	    ./"$programme" "$thread" > /dev/null #stdout vers rien pour pas nuire nos perfs
	    endtime=$(date +%s.%N)
	    totaltime=$(echo "$endtime - $starttime" | bc -l) #bc: basic calculator est là avec -l pour calculer les float sans perte
	    echo "$thread,$i,$totaltime" >> "analyses/${programme}.csv"
	done
	else
	for (( i=1; i<=5; i++)); do
	    divided=$((thread/2))
	    starttime=$(date +%s.%N)
	    ./"$programme" "$divided" "$divided" > /dev/null
	    endtime=$(date +%s.%N)
	    totaltime=$(echo "$endtime - $starttime" | bc -l)
	    echo "$thread,$i,$totaltime" >> "analyses/${programme}.csv"
	done
	fi
    done
done
