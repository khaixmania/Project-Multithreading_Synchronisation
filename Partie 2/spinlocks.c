// Tâche 2.1
// Verrou test-and-set
#include "spinlocks.h"
volatile int x = 0; //variable globale 'verrou'

void lock(){
    int y = 1; //on veut mettre la valeur 1 (locked) dans le verrou
	//Attente active
    while(y==1){
	// elle échange la valeur de y avec la mem x
	__asm__ __volatile__(
            "xchgl %0, %1" // %0 = y et %1 = x, on les 'swap'
            : "+r"(y), "+m"(x) //outputs: y en registre (+r) et x en mémoire (+m)
	    :						// pas d'inputs
	    : "memory"				// signal pour dire que la mem est mise à jour
        );
	//après 'xchgl':
	//si y = 0 (0 swap de x) alors on met x à 1. Nous avons pris le verrou comme il était libre, maintenant la boucle s'arrête
	//si y = 1 (1 swap de x), alors on met x à 1. Comme le verrou est déjà occupé, la boucle continue
    }
}
void unlock(){
    __asm__ __volatile__(
        "movl $0, %0" // on écrit 0 dans la variable x
        : "+m"(x) //output: x en mem
        : 		  //aucuns inputs
        : "memory"
    );
}
