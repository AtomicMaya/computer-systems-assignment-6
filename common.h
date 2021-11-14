/** @author Nicolas Boeckh & Sofia Delijaj */
#ifndef HEADER_COMMON
#define HEADER_COMMON

#include <semaphore.h>

#define QUANTITY    10
#define MAX_TIME    1

#define SHM_NAME    "shm"
#define FILL        "f"
#define AVAILABLE   "a"
#define MUT_EXCL    "m"
#define DONE        "d"

#define SLEEPTIME(r)  (r) % (MAX_TIME) + 1 

sem_t* getSemaphore(char *name, int v);

#endif