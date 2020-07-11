#include "comm.h"


//flag for timeout
int threshold;


int main(int argc,char* argv[])
{
   if(argc!=2)
   {
       errx(1,"Invalid number of arguments");  
   }
   
   //Generating a System V IPC key
   key_t key;
   if((key=ftok(PATHNAME,PROJECT_ID))==-1)
   {
     err(5,"Couldnt create a unique key");
   }

   //openning the shared segment created by the server with size 1024
   //Note how to only connect to the segment without creating it
   int shmid;
   if((shmid=shmget(key,1024,0777))==-1)
   {
     err(6,"Couldnt open the already created shared segment");
   }

   //attaching the segment to the address space of the calling process
   if((content=(char*)shmat(shmid,(void*)0,0))==(char*)(-1))
   {
     err(7,"Couldnt attach the segment to the space of the calling process");
   }
   
   //Opening the semaphore which will signal the server that a message is waiting for him
   if((mutex=sem_open(MUTEX_NAME,0))==SEM_FAILED)
   {
     const int er=errno;
     
     shmdt(content);

     errno=er;
     err(8,"Failed to open semaphore mutex");
   }

   //Opening the sempahore which will signal the child that a message is waiting for him
   if((answer=sem_open(ANSWER_NAME,0))==SEM_FAILED)
   {
     const int er=errno;

     sem_close(mutex);
     shmdt(content);
     
     errno=er;
     err(8,"Failed to open semapthore answer");
   }

    //Opening the clients sempahore, created by the server
   if((clients=sem_open(CLIENTS_NAME,0))==SEM_FAILED)
   {
     const int er=errno;
     
     sem_close(mutex);
     sem_close(answer);
     shmdt(content);
     
     errno=er;
     err(8,"Failed to open semaphore clients");
   }
   
   //Handling the SIGTERM signal of the client
   signal(SIGINT,sigEndHandler);
   signal(SIGALRM,timeToRespond);

   //wait till the server is free
   sem_wait(clients);
   threshold=1;
   memset(content,0,1024);
         
   //send the char, identifing the account
   memcpy(content,argv[1],strlen(argv[1])+1); 
         
   //signals the server that a client has sent a request
   sem_post(mutex);

   //waiting for the answer of the server
   sem_wait(answer);

   //If the account isnt found
   if(strlen(content)==0)
   {
       printf("Invalid number of account\n");
       sem_post(clients);
       sem_close(answer);
       sem_close(mutex);
       sem_close(clients);
       shmdt(content);
       _exit(13);
   }

   printf("The chosen bank account has %s balance\n",content);
   char message[1000];

   memset(&message,0,sizeof(message));
   if(write(1,"Enter a sum:",13)!=13)
   {
       const int er=errno;
       sem_post(clients);
       sem_close(answer);
       sem_close(mutex);
       sem_close(clients);
       shmdt(content);
       errno=er;
       err(12,"Couldnt write on the terminal");
   }
   alarm(16);
   ssize_t readsize=read(0,&message,sizeof(message));
   threshold=0;
   //if its empty string
   if(strcmp(message,"\n")==0)
   {
       message[0]='\0';
   }
         
   memcpy(content,&message,readsize);

   //sending signal to server to read the message
   sem_post(mutex);

   //waiting for an answer from the server
   sem_wait(answer);
         
   int exitSt=0;
   //if it's an empty string then there was an error with the transaction
   if(strlen(content)==0)
   {
        printf("Transaction failed. The balance hasnt been changed.\n");
        exitSt=1;
   }
   else
   {
        printf("Transaction successful.");
        printf("The updated balance is :%s\n",content);
   }
   sem_post(clients);
   sem_close(answer);
   sem_close(mutex);
   sem_close(clients);
   shmdt(content);
   _exit(exitSt);
}

//Function for handling sigterm signals
void sigEndHandler(const int sig_num)
{
    printf("The program will exit. The signal %d has been called\n",sig_num);
    memcpy(content,"",1);
    sem_post(mutex);

    //if we dont have it then the program bugs
    sem_wait(answer);
    
    sem_close(answer);
    sem_close(mutex);

    shmdt(content);
    sem_post(clients);
    sem_close(clients);
    _exit(17);
} 

//Function for handling the timeout of the client 
void timeToRespond(const int sig_num)
{ 
  if(threshold==1)
  {
      printf("You have exeeded the time required to send an answer. Sig_num :%d\n",sig_num);
      memcpy(content,"timeout",8);
      sem_post(mutex);

      sem_wait(answer);
      
      sem_post(clients);
      _exit(18);
  }
}
