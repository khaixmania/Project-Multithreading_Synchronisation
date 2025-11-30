// Tâche 2.5 — Partie 2

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "spinlocks2.h"

int N; //nbr de philosophes
#define CYCLES_P_M 1000000
int *baguettes; //pthread_mutex_t *baguettes; maintenant ce n'est que 0 (libre) ou 1(occupé)

void error(int err, char *msg) {
    fprintf(stderr,"%s a retourné %d, message d’erreur : %s\n",msg,err,strerror(err));
    exit(EXIT_FAILURE);
}
void* philosophe (void* arg){
    int *id = (int *) arg;
    int left = *id;
    int right = (left+1) % N;
    for (int i=0; i < CYCLES_P_M; i++){
	// On prend les baguettes (lock) avec lock2() avec de l'attente active
	// Si une des baguettes est prise, le threade va boucler à l'infini tout en consommant le CPU
	if (left<right){//ordre forcée d'abord le plus petit
	    lock2(&baguettes[left]);
	    lock2(&baguettes[right]);
	}
	else{
	    lock2(&baguettes[right]);
	    lock2(&baguettes[left]);
	}
	//manger() constant
	unlock2(&baguettes[left]);
	unlock2(&baguettes[right]);
	//penser() constant
    }
    return (NULL);
}

int main (int argc, char *argv[]){
    int err;
    if (argc != 2){
	fprintf(stderr, "Avoir: %s <N>", argv[0]);
	exit(EXIT_FAILURE);
    }
    N = atoi(argv[1]);
    if (N<=1){
	fprintf(stderr, "N>1");
	exit(EXIT_FAILURE);
    }
    int *thread_id = malloc(N*sizeof(int));
    pthread_t *nthreads = malloc(N*sizeof(pthread_t));
    baguettes = malloc(N*sizeof(pthread_mutex_t));

    for (int i=0; i<N; i++){baguettes[i] = 0;} // ici, on init les verrous à 0 (verrous libres)

    for (int i=0; i<N; i++){
	thread_id[i] = i;
	err = pthread_create(&nthreads[i], NULL, philosophe,(void *)&thread_id[i]);
	if (err != 0) error(err, "pthread_create");
    }
    for (int i=0; i<N; i++){
	err = pthread_join(nthreads[i], NULL);
	if (err != 0)error(err, "pthread_join");
    }
    //plus besoin de destroy des semaphores, on a des int maintenant
    free(thread_id);
    free(baguettes);
    free(nthreads);
    return(EXIT_SUCCESS);
}
