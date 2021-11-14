/** @author Nicolas Boeckh & Sofia Delijaj */

#include <semaphore.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>

sem_t* getSemaphore(char *name, int v) {
    sem_t *sem;
    sem = sem_open(name, O_CREAT, 0666, v);
    return sem;
}