//+
//	ELEC 377,  Lab 3
//
//  producer.c
//-
//+
//Brief description of producer
//The producer caints the code to access the shared memory segment and maps it to memory
//The producer will copy the input to the shared memory by one byte at a time
//-

#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <errno.h>

#include "common.h"

#define FALSE 0
#define TRUE 1


int main (int argc, char *argv[]){

	// initialize the shared memory, load in the initial array's, spawn the worker
	// processes.

	key_t   key;
	struct shared    *sharedPtr;
	int shmid;
	int c,stored;
    
	/*	 Shared memory init 	*/
	key = ftok(".", 'S');
	if((shmid = shmget(key, MEMSIZE, IPC_CREAT|0666)) == -1 ){
		if( (shmid = shmget(key, MEMSIZE, 0)) == -1){
			printf("Error allocating shared memory. \n");
			exit(1);
		}
	}

	// now map the region..
	if((int)(sharedPtr = (struct shared *) shmat(shmid, 0, 0)) == -1){
	    printf("Couldn't map the memory into our process space.\n");
	    exit(1);
	}
	//+
	//Brief description of code written
	//while loop will read each character using getchar function until it reaches the EOF
	//Nested in while loop is that requests access to critical section and checks if there is room in queue
	//If there is room in queue it adds character to queue and then releases mutex
	//Inner loop is controlled by a flag that is set whenever the producer adds a byte
	//-
    // put your code here..
	getMutex(&sharedPtr->lock);
	sharedPtr->numProducers++;
	releaseMutex(&sharedPtr->lock);

	while((c = getchar()) != EOF){
		stored = FALSE;
		while(stored == FALSE){
			getMutex(&sharedPtr->lock);
				if(sharedPtr->count < BUFFSIZE){
					sharedPtr->buffer[sharedPtr->in] = c;
					sharedPtr->in = (sharedPtr->in + 1) %BUFFSIZE;
					(sharedPtr->count)++;
					stored = TRUE;
				}
				releaseMutex(&sharedPtr->lock);
		}
	}
	getMutex(&sharedPtr->lock);
	sharedPtr->numProducers--;
	releaseMutex(&sharedPtr->lock);


	return 0;
}


