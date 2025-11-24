// Tâche 1.1 — Partie 1

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int N;
#define CYCLES_P_M 1000000
pthread_mutex_t *baguettes;

void error(int err, char *msg) {
    fprintf(stderr,"%s a retourné %d, message d’erreur : %s\n",msg,err,strerror(err));
    exit(EXIT_FAILURE);
}
void* philosophe (void* arg){
    int *id = (int *) arg;
    int left = *id;
    int right = (left+1) % N;
    for (int i=0; i < CYCLES_P_M; i++){
	if (left<right){
	    pthread_mutex_lock(&baguettes[left]);
	    pthread_mutex_lock(&baguettes[right]);
	}
	else{
	    pthread_mutex_lock(&baguettes[right]);
	    pthread_mutex_lock(&baguettes[left]);
	}
	pthread_mutex_unlock(&baguettes[left]);
	pthread_mutex_unlock(&baguettes[right]);
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
    for (int i=0; i<N; i++){
	err = pthread_mutex_init(&baguettes[i], NULL);
	if (err != 0) error(err, "pthread_mutex_init");
    }
    for (int i=0; i<N; i++){
	thread_id[i] = i;
	err = pthread_create(&nthreads[i], NULL, philosophe,(void *)&thread_id[i]);
	if (err != 0) error(err, "pthread_create");
    }
    for (int i=0; i<N; i++){
	err = pthread_join(nthreads[i], NULL);
	if (err != 0)error(err, "pthread_join");
    }
    for (int i=0; i<N; i++){
	err = pthread_mutex_destroy(&baguettes[i]);
	if (err != 0)error(err, "pthread_mutex_destroy");
    }
    free(thread_id);
    free(baguettes);
    free(nthreads);
    return(EXIT_SUCCESS);
}
