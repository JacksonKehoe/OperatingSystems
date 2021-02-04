//+
//  common.h - Common definiton of the shared
//     memory segment and prototypes for semaphore functions
// 
//  Created by Thomas Dean
//  Copyright 2005 Queen's University.
//
//-

#define MEMSIZE 200
#define BUFFSIZE 5

//+
//creating shared structure which is shared memory
//- 
struct shared {

    int lock;
    char buffer[BUFFSIZE];
    int in;
    int out;
    int count;
    int numProducers;
};

void getMutex(int * lock);
void releaseMutex(int * lock);
