#include "my_semaphore.h"
void my_sem_init(my_sem_t *sem, int val){
    sem->x = val;
    sem->verrou = 0; //verrou est libre à l'init
}

void my_sem_wait(my_sem_t *sem){
    while(1){
	lock2(&sem->verrou); //lock l'accès à la structure du sémaphore
	if (sem->x > 0){ // voir si ressource est dispo
	    sem->x--; //on la prend (donc dec)
	    unlock2(&sem->verrou); //puis on libère du compteur x
	    break; //fin
	}
	unlock2(&sem->verrou); //en cas d'absence de ressource, on post le verrou pour d'autres
    }
}

void my_sem_post(my_sem_t *sem){
    lock2(&sem->verrou); //lock
    sem->x++; //ajouter une ressource (inc)
    unlock2(&sem->verrou); //unlock
}
