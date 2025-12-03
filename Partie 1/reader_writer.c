// Tâche 1.3 — Partie 1
// Lecteurs-Ecrivains

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#define TOTAL_READ 25400
#define TOTAL_WRITE 6400

pthread_mutex_t mrc; //mutex pour rc
pthread_mutex_t mwc; //mutex pour wc
pthread_mutex_t z; //mutex pour une file d'attente, permet d'éviter de laisser passer les nouveaux lecteurs si un écrivain arrive
int rc = 0; //nombre de lecteurs actifs
int wc = 0; //nombre d'écrivains actifs
sem_t rsem; //bloque les lecteurs quand un écrivain est occupé
sem_t wsem; 
int nb_writes;
int nb_reads;

void error(int err, char *msg) {
    fprintf(stderr, "%s a retourné %d, message d’erreur : %s\n", msg, err, strerror(err));
    exit(EXIT_FAILURE);
}

void* writer(void* arg){
    int err;
    for (int i=0; i<nb_writes; i++){
	//ecrivain qui arrive
	if ((err = pthread_mutex_lock(&mwc)) != 0) error(err, "mutex_lock mwc");
        wc++;
		//l'écrivain qui arrive bloque l'accès aux lecteurs avec sem_wait(&rsem) comme wc = 1
        if (wc == 1) sem_wait(&rsem);
        if ((err = pthread_mutex_unlock(&mwc)) != 0) error(err, "mutex_unlock mwc");
	//wait l'acces au ressource
	sem_wait(&wsem);
	//SECTION CRITIQUE
	for (int i = 0; i < 10000; i++); //traitement
	sem_post(&wsem);
    //ecrivain qui va sortir
	if ((err = pthread_mutex_lock(&mwc)) != 0) error(err, "mutex_lock mwc");
	wc--;
		// l'écrivain qui sort ne bloque plus les lecteurs
        if (wc == 0) sem_post(&rsem); //dernier des writers cède sa place au lecteur
        if ((err = pthread_mutex_unlock(&mwc)) != 0) error(err, "mutex_unlock mwc");
    }
    return NULL;
}


void *reader(void* arg){
    int err;
    for (int i=0; i<nb_reads; i++){
	//lecteur qui arrive
	//ici on regarde le z, si il est déjà pris par un écrivain, on va rester ici
	if ((err = pthread_mutex_lock(&z)) != 0) error(err, "mutex_lock z");
	sem_wait(&rsem);
	if ((err = pthread_mutex_lock(&mrc)) != 0) error(err, "mutex_lock mrc");
	rc++;
	//le lecteur va lock la ressource wsem pour bloquer l'accès aux écrivains
	if (rc == 1) sem_wait(&wsem);
	if ((err = pthread_mutex_unlock(&mrc)) != 0) error(err, "mutex_unlock mrc");
	//le prochain lecteur est laissé rentré 
	sem_post(&rsem);
	if ((err = pthread_mutex_unlock(&z)) != 0) error(err, "mutex_unlock z");
	//SECTION CRITIQUE
	for (int i = 0; i < 10000; i++);//traitement
	//lecteur qui va sortir
	if ((err = pthread_mutex_lock(&mrc)) != 0) error(err, "mutex_lock mrc");
	rc--;
	if (rc == 0) sem_post(&wsem);// dernier des lecteurs cède la place à l'écrivain
	if ((err = pthread_mutex_unlock(&mrc)) != 0) error(err, "mutex_unlock mrc");
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    int err;
    if (argc != 3) {
        fprintf(stderr, "Avoir: %s <n_writers> <n_readers>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int n_writers = atoi(argv[1]);
    int n_readers = atoi(argv[2]);

    if (n_writers <= 0 || n_readers <= 0) {
        fprintf(stderr, "Arguments(reader ou writer) > 0\n");
        exit(EXIT_FAILURE);
    }

    if (n_writers > 0) nb_writes = TOTAL_WRITE / n_writers;

    else{nb_writes = 0;}

    if (n_readers > 0) nb_reads = TOTAL_READ / n_readers;
    else{nb_reads = 0;}

    pthread_mutex_init(&mrc, NULL);
    pthread_mutex_init(&mwc, NULL);
    pthread_mutex_init(&z, NULL);

    if (sem_init(&rsem, 0, 1) != 0){perror("sem_init rsem"); exit(EXIT_FAILURE);}
    if (sem_init(&wsem, 0, 1) != 0){perror("sem_init wsem"); exit(EXIT_FAILURE);}

    pthread_t *writers = malloc(n_writers * sizeof(pthread_t));
    pthread_t *readers = malloc(n_readers * sizeof(pthread_t));

    for (int i = 0; i < n_writers; i++) {
        if ((err = pthread_create(&writers[i], NULL, writer, NULL)) != 0)error(err, "pthread_create writer");
    }
    for (int i = 0; i < n_readers; i++) {
        if ((err = pthread_create(&readers[i], NULL, reader, NULL)) != 0)error(err, "pthread_create reader");
    }
    for (int i = 0; i < n_writers; i++) pthread_join(writers[i], NULL);
    for (int i = 0; i < n_readers; i++) pthread_join(readers[i], NULL);

    pthread_mutex_destroy(&mrc);
    pthread_mutex_destroy(&mwc);
    pthread_mutex_destroy(&z);
    sem_destroy(&rsem);
    sem_destroy(&wsem);
    free(writers);
    free(readers);
    return EXIT_SUCCESS;
}
