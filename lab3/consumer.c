//+
//    ELEC 377,  Lab 3
//
//  consumer.c
//-
//+
//Brief Description of Consumer
//Similar to producer the code maps the shared memory segment into memory
//This aids in the output by copying the data from the shared memory to the output by 1 byte at a time
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
//+
//main function
//-
int main (int argc, char *argv[]){

	// initialize the shared memory, load in the initial array's, spawn the worker
	// processes.

	key_t   key;
	struct shared    *sharedPtr;
	int shmid;
    
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
	//The consumer transfers a single character form the queue to the output. 
	//A while loop is used to retrieve each character
	//A nested loop is used to indicate when a byte has been sucessfully retrieved from the buffer
	//The code exits when the number of producers is 0 because there are no bytes left to transfer
	//-
    // put your code here...
getMutex(&sharedPtr->lock);
    int numProd = sharedPtr->numProducers;
    releaseMutex(&sharedPtr->lock);

    int charRead = TRUE;
    int c;
    int lastChar = FALSE;

    while((numProd > 0) && (charRead == TRUE)){
    	charRead = FALSE;

    	while((numProd > 0) && (charRead == FALSE)){
    		getMutex(&sharedPtr->lock);
			if(sharedPtr->count > 0){
    			c = sharedPtr->buffer[sharedPtr->out]; 
    			sharedPtr->out = (sharedPtr->out +1) % BUFFSIZE;
    			(sharedPtr->count)--; 
    			charRead = TRUE; 
                lastChar = FALSE;
    		} else {
                lastChar = TRUE;
    			numProd = sharedPtr->numProducers; 
    		}

    		releaseMutex(&sharedPtr->lock);
    	}
    	if(lastChar != TRUE) putchar(c);
    }
    
	return 0;
}


