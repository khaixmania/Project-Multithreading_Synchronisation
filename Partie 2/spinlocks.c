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
    x = 0;
}
