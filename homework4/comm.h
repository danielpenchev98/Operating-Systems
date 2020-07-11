#ifndef COMM_H
#define COMM_H

#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>  
#include <sys/wait.h>
#include <sys/shm.h> 
#include <errno.h>     
#include <semaphore.h>  
#include <fcntl.h>      
#include <string.h> 
#include <err.h>
#include <signal.h>

//names of the semaphores
#define MUTEX_NAME "ServerSemaphore"
#define ANSWER_NAME "AnswerSemaphore"
#define CLIENTS_NAME "SemClients"

//Needed to generate the unique key
#define PATHNAME "."
#define PROJECT_ID 5

//semaphore pointers
sem_t* clients;
sem_t* mutex;
sem_t* answer;
char *content;

//Needed to handle cntr+C signal
void sigEndHandler(const int sig_num);

//Needed to handle the timeout of the client
void timeToRespond(const int sig_num);

#endif