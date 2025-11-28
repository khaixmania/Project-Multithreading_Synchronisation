#include "spinlocks_ttas.h"

void lock2(int *arg){
    while (1) {
        while (*arg == 1){}
        int y = 1;
        __asm__ __volatile__(
            "xchgl %0, %1"
            : "+r"(y), "+m"(*arg)
            :
            : "memory"
        );
        if (y == 0) {return;}
    }
}

void unlock2(int *arg) {
    __asm__ __volatile__(
        "movl $0, %0"
        : "+m" (*arg)
        :
        : "memory"
    );
}
