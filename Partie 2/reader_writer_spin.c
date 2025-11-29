// Tâche 2.5 — Partie 2

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#define TOTAL_READ 25400
#define TOTAL_WRITE 6400

int mrc = 0; //pthread_mutex_t mrc;
int mwc = 0; //pthread_mutex_t mwc;
int z = 0;//pthread_mutex_t z;
int rc = 0;
int wc = 0;
my_sem_t rsem;
my_sem_t wsem;
int nb_writes;
int nb_reads;

void error(int err, char *msg) {
    fprintf(stderr, "%s a retourné %d, message d’erreur : %s\n", msg, err, strerror(err));
    exit(EXIT_FAILURE);
}

void* writer(void* arg){
    int err;
    for (int i=0; i<nb_writes; i++){
	lock2(&mwc);
        wc++;
        if (wc == 1)my_sem_wait(&rsem);
	unlock2(&mwc);
	my_sem_wait(&wsem);
	//SC
	for (int i = 0; i < 10000; i++); //traitement
	my_sem_post(&wsem);
	lock2(&mwc);
	wc--;
        if (wc == 0) my_sem_post(&rsem); //dernier des writers cède sa place au lecteur
	unlock2(&mwc);
    }
    return NULL;
}


void *reader(void* arg){
    for (int i=0; i<nb_reads; i++){
	lock2(&z);
	my_sem_wait(&rsem);
	lock2(&mrc);
	rc++;
	if (rc == 1) my_sem_wait(&wsem);
	unlock2(&mrc);
	my_sem_post(&rsem);
	unlock2(&z);
	//SC
	for (int i = 0; i < 10000; i++);//traitement
	lock2(&mrc);
	rc--;
	if (rc == 0) my_sem_post(&wsem);// dernier des lecteurs cède la place à l'écrivain
	unlock2(&mrc);
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

    mrc = 0;
    mwc = 0;
    z = 0;
    my_sem_init(&rsem, 1);
    my_sem_init(&wsem, 1);

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

    //pthread_mutex_destroy(&mrc);
    //pthread_mutex_destroy(&mwc);
    //pthread_mutex_destroy(&z);
    //sem_destroy(&rsem);
    //sem_destroy(&wsem);
    free(writers);
    free(readers);
    return EXIT_SUCCESS;
}
