// Tâche 2.1
// Verrou test-and-set
#include "spinlocks.h"
volatile int x = 0; //variable globale 'verrou' et volatile permet de ne pas le stocker dans un registre

/**
 * Acquiert le verrou (Attente active).
 * Principe : L'algorithme test-and-set tente d'écrire atomiquement la valeur '1'
 * dans le verrou. Si l'ancienne valeur était 0, c'est réussi. Sinon, on réessaie.
 */
void lock(){
    int y = 1; //on veut mettre la valeur 'locked' dans le verrou
	//Attente active
    while(y==1){
	// asm inline: elle échange la valeur de y avec la mémoire x.
	__asm__ __volatile__(
            "xchgl %0, %1" // %0 = y et %1 = x
            : "+r"(y), "+m"(x) //outputs: y en registre (+r) et x en mémoire (+m)
	    :						// pas d'inputs
	    : "memory"				// signal pour dire que la mémoire a été modifié
        );
	// après 'xchgl', si y = 0 (0 venant de x) alors x = 1. Nous avons pris le verrou comme il était libre, maintenant la boucle s'arrête (y != 1)
	//				, si y = 1 (1 venant de x), alors x = 1. Comme le verrou est déjà occupé, la boucle continue
    }
}
/**
 * Libèrer le verrou.
 * Principe : On remet la variable globale à 0.
 */
void unlock(){
    __asm__ __volatile__(
        "movl $0, %0" // on écrit 0 dans la variable x
        : "+m"(x) //output: x en mémoire
        : 		  //aucuns inputs
        : "memory"
    );
}
