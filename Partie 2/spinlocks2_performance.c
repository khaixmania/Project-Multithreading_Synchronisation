#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/time.h>
#include "spinlocks2.h"

long section_critique;
int verrou = 0;

void *function(void *arg) {
    for (int j=0; j<section_critique; j++) {
        lock2(&verrou);
        for (int i=0; i<10000; i++);
        unlock2(&verrou);
    }
    return NULL;
}

int main(int argc, char* argv[]){
    if (argc != 2){
        fprintf(stderr, "Avoir: %s N", argv[0]);
        return (EXIT_FAILURE);
    }
    int N = atoi(argv[1]);
    if (N <= 0){
        fprintf(stderr, "N doit être > 0");
        return (EXIT_FAILURE);
    }
    section_critique = (32768/N);
    verrou = 0;
    pthread_t *threads = malloc(N*sizeof(pthread_t));
    struct timeval t_start, t_end;
    gettimeofday(&t_start, NULL);
    for (int i=0; i<N; i++){pthread_create(&threads[i], NULL, function, NULL);}
    for (int i=0; i<N; i++){pthread_join(threads[i], NULL);}
    gettimeofday(&t_end, NULL);
    double totalsec = (t_end.tv_sec - t_start.tv_sec) + (t_end.tv_usec - t_start.tv_usec) / 1e6;
    printf("N = %d, temps = %f secondes", N, totalsec);
    free(threads);
    return EXIT_SUCCESS;
}
