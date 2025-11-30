// Tâche 2.5 — Partie 2
// Producteurs-Consommateurs avec les spinlocks
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include "spinlocks2.h"
#include "my_semaphore.h"

#define MAX_ELEMENTS 131072
#define N 8

int mutex = 0; //pthread_mutex_t mutex;
my_sem_t empty; //sem pour les places libres
my_sem_t full; //sem pour les places occupées
int id_write = 0;
int id_read = 0;
int elem_produce;
int elem_consume;
int buffer[N];//buffer: tab de 8 places avec int

void error(int err, char *msg) {
    fprintf(stderr,"%s a retourné %d, message d’erreur : %s\n",msg,err,strerror(err));
    exit(EXIT_FAILURE);
}

void* producer(void* arg){
    int err;
    int id = *(int*)arg; //chaque thread insère un int fixe dans le buffer -> id
    for (int j=0; j<elem_produce; j++){
	for (int i=0; i<10000; i++);//traitement en dehors de SC
	//attente active d'une place libre, si le buffer est plein alors le thread boucle au max sur le CPU
	my_sem_wait(&empty);
	lock2(&mutex); //commence SC, attente active si c'est déjà occupé
	buffer[id_write] = id;
	id_write++;
	if (id_write == N) id_write = 0;
	unlock2(&mutex);//termine SC
	my_sem_post(&full);//signale une place occupée
    }
    return (NULL);
}

void* consumer(void* arg){
    for (int j=0; j<elem_consume; j++){
	//attente active d'une donnée, si le buffer est vide alors le thread saturera un core pour rien
	my_sem_wait(&full);
	lock2(&mutex); //SC
	id_read++;
	if (id_read == N) id_read = 0;
	unlock2(&mutex);// Fin de la SC
	my_sem_post(&empty);
	for (int i=0; i<10000; i++);
    }
    return (NULL);
}

int main(int argc, char *argv[]) {
    int err;

    if (argc != 3) {
        fprintf(stderr, "Avoir: %s <producers> <consumers>", argv[0]);
        exit(EXIT_FAILURE);
    }
    int p_size = atoi(argv[1]);
    int c_size = atoi(argv[2]);

    if (p_size <= 0 || c_size <= 0) {
        fprintf(stderr, "Args > 0");
        exit(EXIT_FAILURE);
    }

    elem_produce = MAX_ELEMENTS / p_size;
    elem_consume = MAX_ELEMENTS / c_size;

    mutex = 0; //verrou libre
    my_sem_init(&empty, N); //N places vides a l'init
    my_sem_init(&full, 0); //Aucune place prise a l'init

    pthread_t *producers = malloc(p_size * sizeof(pthread_t));
    pthread_t *consumers = malloc(c_size * sizeof(pthread_t));
    int *total_id = malloc(p_size * sizeof(int));

    for (int i = 0; i < p_size; i++) {
        total_id[i] = i;
        if ((err = pthread_create(&producers[i], NULL, producer, &total_id[i])) != 0)error(err, "pthread_create producers");
    }

    for (int i = 0; i < c_size; i++) {
        if ((err = pthread_create(&consumers[i], NULL, consumer, NULL)) != 0)error(err, "pthread_create consumers");
    }

    for (int i = 0; i < p_size; i++) pthread_join(producers[i], NULL);
    for (int i = 0; i < c_size; i++) pthread_join(consumers[i], NULL);

    // pthread_mutex_destroy(&mutex);
    //sem_destroy(&empty);
    //sem_destroy(&full);
    free(producers);
    free(consumers);
    free(total_id);
    return (EXIT_SUCCESS);
}
