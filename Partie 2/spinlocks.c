//Tâche 2.1
#include "spinlocks.h"
volatile int x = 0;

void lock(){
    int y = 1;
    while(y==1){
	__asm__ __volatile__(
            "xchgl %0, %1"
            : "+r"(y), "+m"(x)
	    :
	    : "memory"
        );
    }
}
void unlock(){
    __asm__ __volatile__(
        "movl $0, %0"
        : "+m"(x)
        :
        : "memory"
    );
}
