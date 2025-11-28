#!/bin/bash

THREADS=(1 2 4 8 16 32)
FILES=("test1" "test2")

mkdir -p "analyses2"
for programme in "${FILES[@]}"; do
    echo "thread,essai n°,time" > "analyses2/${programme}.csv"
    if [ "$programme" == "test1" ]; then
        gcc -Wall -o "$programme" spinlocks_performance.c spinlocks.c -lpthread
    else
        gcc -Wall -o "$programme" performance_ttas.c spinlocks_ttas.c -lpthread
    fi
    for thread in "${THREADS[@]}"; do
        if [ "$programme" == "test1" ]; then
            for (( i=1; i<=5; i++)); do
                starttime=$(date +%s.%N)
                ./"$programme" "$thread" > /dev/null
                endtime=$(date +%s.%N)
                totaltime=$(echo "$endtime - $starttime" | bc -l)
                echo "$thread,$i,$totaltime" >> "analyses2/${programme}.csv"
            done
        else
            for (( i=1; i<=5; i++)); do
                starttime=$(date +%s.%N)
                ./"$programme" "$thread" > /dev/null
                endtime=$(date +%s.%N)
                totaltime=$(echo "$endtime - $starttime" | bc -l)
                echo "$thread,$i,$totaltime" >> "analyses2/${programme}.csv"
            done
        fi
    done
done
