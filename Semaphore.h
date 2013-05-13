#ifndef SEMAPHORE
#define SEMAPHORE
#include "semaphore.h"

int sem_init (sem_t *s, int p, unsigned int v);
int sem_destroy (sem_t *s);
int sem_getvaleur (sem_t* sem, int * sval);
int sem_wait(sem_t * sem);
int sem_post(sem_t * sem);

#endif
