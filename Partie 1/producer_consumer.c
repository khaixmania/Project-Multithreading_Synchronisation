// Tâche 1.2 — Partie 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#define MAX_ELEMENTS 131072
#define N 8

pthread_mutex_t mutex;
sem_t empty;
sem_t full;
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
	for (int i=0; i<10000; i++);
	sem_wait(&empty);
	err = pthread_mutex_lock(&mutex);
	if (err != 0)error(err, "mutex_lock");
	//SC
	buffer[id_write] = id;
	id_write++;
	if (id_write == N) id_write = 0;
	err = pthread_mutex_unlock(&mutex);
	if (err != 0)error(err, "mutex_unlock");
	sem_post(&full);
    }
    return (NULL);
}

void* consumer(void* arg){
    int err;
    for (int j=0; j<elem_consume; j++){
	sem_wait(&full);
	err = pthread_mutex_lock(&mutex);
	if (err != 0)error(err, "mutex_lock");

	//SC
	id_read++;
	if (id_read == N) id_read = 0;
	err = pthread_mutex_unlock(&mutex);
	if (err != 0)error(err, "mutex_unlock");
	sem_post(&empty);
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

    if (pthread_mutex_init(&mutex, NULL) != 0) error(0, "mutex_init");
    if (sem_init(&empty, 0, N) != 0){perror("sem_init empty"); exit(EXIT_FAILURE);}
    if (sem_init(&full, 0, 0) != 0){perror("sem_init full"); exit(EXIT_FAILURE);}

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

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    free(producers);
    free(consumers);
    free(total_id);
    return (EXIT_SUCCESS);
}
