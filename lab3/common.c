//+ 	ELEC 377, Lab 3
//
//   common.c contains routines to be used from both the
//   producer, and the  consumer
//   Mutual Exclusion routines will be here
//-

#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <asm/system.h>


#include "common.h"

#define FALSE 0
#define TRUE 1
//+
//function that helps lock processes in the critical section
//no other function can execute it's critical section while one process is in test and set
//-
int test_and_set(int * lock){
    return __cmpxchg(lock,0,1,4);
}
//+
//the locking mechanism function
//-
void getMutex(int *  lock){
	// this should not return until it has mutual exclusion. Note that many versions of 
	// this will probobly be running at the same time.
	while(test_and_set(lock));
}
//+
//unlocking mechanism function
//-
void releaseMutex(int * lock){
	// set the mutex back to initial state so that somebody else can claim it
	*lock = FALSE;
}

