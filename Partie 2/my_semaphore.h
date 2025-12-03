#include "spinlocks2.h" //Choix de la meilleure logique (test-and-test-and-set)
typedef struct {
    volatile int x; //compteur pour le nbr de ressourcess dispo
    int verrou;
} my_sem_t;
//init sémpahore avec une valeur
void my_sem_init(my_sem_t *sem, int val);

//remplacer sem_wait, decremente x
void my_sem_wait(my_sem_t *sem);

//remplacer sem_post, incremente x
void my_sem_post(my_sem_t *sem);
