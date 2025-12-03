#!/bin/bash
# Tâches 2.2-2.3
THREADS=(1 2 4 8 16 32)
FILES=("test1" "test2")

mkdir -p "analyses2"
for programme in "${FILES[@]}"; do
    echo "thread,essai n°,time" > "analyses2/${programme}.csv"
    if [ "$programme" == "test1" ]; then
        gcc -Wall -o "$programme" spinlocks_performance.c spinlocks.c -lpthread
    else
        gcc -Wall -o "$programme" spinlocks2_performance.c spinlocks2.c -lpthread
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

#La suite ici
#Tâche 2.5
THREADS2=(2 4 8 16 32)
FILES2=("philosophers_spin" "producer_consumer_spin" "reader_writer_spin")

for programme in "${FILES2[@]}"; do
    echo "thread,essai n°,time" > "analyses2/${programme}.csv"
    if [ "$programme" == "philosophers_spin" ]; then
        gcc -Wall -o "$programme" philosophers_spin.c spinlocks2.c -lpthread
    elif [ "$programme" == "producer_consumer_spin" ]; then
        gcc -Wall -o "$programme" producer_consumer_spin.c spinlocks2.c my_semaphore.c -lpthread
    else
        gcc -Wall -o "$programme" reader_writer_spin.c spinlocks2.c my_semaphore.c -lpthread
    fi
    for thread in "${THREADS2[@]}"; do
	if [ "$programme" == "philosophers_spin" ]; then
	for (( i=1; i<=5; i++)); do
            starttime=$(date +%s.%N)
            ./"$programme" "$thread" > /dev/null
            endtime=$(date +%s.%N)
            totaltime=$(echo "$endtime - $starttime" | bc -l)
            echo "$thread,$i,$totaltime" >> "analyses2/${programme}.csv"
        done
        else
        for (( i=1; i<=5; i++)); do
            divided=$((thread/2))
            starttime=$(date +%s.%N)
            ./"$programme" "$divided" "$divided" > /dev/null
            endtime=$(date +%s.%N)
            totaltime=$(echo "$endtime - $starttime" | bc -l)
            echo "$thread,$i,$totaltime" >> "analyses2/${programme}.csv"
        done
        fi
    done
done
