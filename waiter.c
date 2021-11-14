/** @author Nicolas Boeckh & Sofia Delijaj */
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <semaphore.h>

#include "common.h"

int main(int argc, char **argv) {
    // Exit case
    int done = 0;

    // Memory element
    int *place;

    // Setup Semaphores
    sem_t   *_fill = getSemaphore(FILL, 0), 
            *_available = getSemaphore(AVAILABLE, 3), 
            *_mutualExclusion = getSemaphore(MUT_EXCL, 1),
            *_done = getSemaphore(DONE, 0);

    // Open memory segment.
    int shm = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm, sizeof(int));

    // Assign memory address.
    place = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);

    do {
        sem_wait(_fill);
        sleep(SLEEPTIME(rand()));
        
        sem_wait(_mutualExclusion);
        
        // Remove element from place
        (*place)--;

        sem_post(_mutualExclusion);
        printf("WAITER: TAKES PIZZA |=| %d LEFT ON SHELF\n", *place);
        sem_post(_available);

        sem_getvalue(_done, &done);
    } while (done == 0 || *place != 0);

    // Memory clean.
    sem_close(_fill);
    sem_close(_available);
    sem_close(_mutualExclusion);
    sem_close(_done);

    sem_unlink(FILL);
    sem_unlink(AVAILABLE);
    sem_unlink(MUT_EXCL);
    sem_unlink(DONE);

  	munmap(place, sizeof(int));
	close(shm);
	shm_unlink(SHM_NAME);
	
    return EXIT_SUCCESS;
}