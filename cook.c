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

/** @author Nicolas Boeckh & Sofia Delijaj */
#include "common.h"

int main(int argc, char **argv) {
    int quantity = QUANTITY;
    
    // Memory element
    int *place;

    // Setup Semaphores
    sem_t   *_fill = getSemaphore(FILL, 0), 
            *_available = getSemaphore(AVAILABLE, 3), 
            *_mutualExclusion = getSemaphore(MUT_EXCL, 1),
            *_done = getSemaphore(DONE, 0);     // Tell server his job is done, he shouldn't know how many pizzas he has to carry around.

    // Open memory segment.
    int shm = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm, sizeof(int));

    // Assign memory address.
    place = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);

    printf("COOK STARTS LOOP\n");

    while (quantity != 0) {
        sem_wait(_available);
        if (quantity != QUANTITY)
            sleep(SLEEPTIME(rand()));
        
        sem_wait(_mutualExclusion);

        // Add element to place
        (*place)++;

        sem_post(_mutualExclusion);

        // Track changes.
        printf("COOK : ADDED NEW PIZZA |=| NOW %d PIZZAS\n", *place);
        
        sem_post(_fill);
        quantity -= 1;
    }
    sem_post(_done);

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