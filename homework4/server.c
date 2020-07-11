#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <sys/stat.h>
#include "comm.h"

uint32_t GetBalance(const int index,const char* const database);
bool Transaction(const int index,const int16_t val,const char* const database,uint32_t * balance);
unsigned char * ConvertDecimalToHex(uint32_t value);
void CreateDB(const char *const DB);
bool CheckFormat(const char* const DB);

int shmid;

int main(int argc, char * argv[])
{
   if(argc!=2)
   {
       errx(1,"Invalid number of arguments");
   }
   const char* const DB=argv[1];

   //checking if the database file exists
   struct stat buff;
   stat(DB,&buff);
   if(!S_ISREG(buff.st_mode))
   {
   	  CreateDB(DB);
   }
   else if(!CheckFormat(DB))
   {
      errx(2,"Invalid format of database");  
   }

   //Generating a System V IPC key
   key_t shmkey;
   if((shmkey=ftok(PATHNAME,PROJECT_ID))==-1)
   {
      err(5,"Couldnt create a unique key");
   }

   //creating the shared segment with size 1024
   if((shmid=shmget(shmkey,1024,0777|IPC_CREAT|IPC_EXCL))==-1)
   {
     err(6,"Couldnt create a new shared segment");
   }

   //attaching the segment to the address space of the calling process
   if((content=(char *)shmat(shmid,(void*)0,0))==(char*)(-1))
   {
     const int er=errno;
     shmctl(shmid,IPC_RMID,NULL);
     errno=er;
     err(7,"Couldnt attach the segment to the space of the calling process");
   }

   //Creating the mutex semaphore which will determine if its server's turn 
   if((mutex=sem_open(MUTEX_NAME,O_CREAT,0777,0))==SEM_FAILED)
   { 
     const int er=errno;
     
     shmdt(content);
     shmctl(shmid,IPC_RMID,NULL);
     
     errno=er;
     err(8,"Failed to open semaphore mutex");
   }
   //Creating the answer semaphore which will determine if its client's turn 
   if((answer=sem_open(ANSWER_NAME,O_CREAT,0777,0))==SEM_FAILED)
   {
     const int er=errno;

     sem_close(mutex);
     sem_unlink(MUTEX_NAME);

     shmdt(content);
     shmctl(shmid,IPC_RMID,NULL);
     
     errno=er;
     err(8,"Failed to open semaphore answer");
   }

   if((clients=sem_open(CLIENTS_NAME,O_CREAT,0777,1))==SEM_FAILED)
   {
     const int er=errno;
     
     sem_close(mutex);
     sem_unlink(MUTEX_NAME);

     sem_close(answer);
     sem_unlink(ANSWER_NAME);

     shmdt(content);
     shmctl(shmid,IPC_RMID,NULL);

     errno=er;
     err(8,"Failed to open semaphore clients");
   }

   //Handling the SIGTERM/SIGINT signal of the server
   signal(SIGINT,sigEndHandler);
   signal(SIGTERM,sigEndHandler);
   while(1)
   {
   	    //every new client signals the server that a request has been sent
        sem_wait(mutex);

        //Getting the position of the account        
        int index=content[0]-'A';
        //message buffer
   	char message[1000];

        //checking if the position is legit
   	if(strlen(content)==1&&index>=0&&index<=8)
  	{
      	    uint32_t balance=GetBalance(index,DB);
      	    sprintf(message,"%d",balance);
      	    memcpy(content,&message,strlen(message)+1); 
  	}
  	else
  	{
        //if it's not then the server sends empty string and continues with the next client
      	    memcpy(content,"",1);
            sem_post(answer);
            continue;
  	}
        //server signals the client that an answer has been sent
        sem_post(answer);
        
        sem_wait(mutex);
        
        //If the client exeeded the max time to respond or the message is the empty
        if(strcmp(content,"")==0||strcmp(content,"timeout")==0)
        {
          //if the user has entered empty string instead of a number
          memcpy(content,"",1);
          sem_post(answer);
          continue;
        }

        int val=atoi(content);
        if(val>=SHRT_MAX||val<=SHRT_MIN)
        {
            memcpy(content,"\0",2);
            sem_post(answer);
           	continue;
        }
        
        //Making the transaction
        uint32_t balance=0;
        if(!Transaction(index,(int16_t)val,DB,&balance))
        {
              sprintf(message,"%d",balance);
        }
        else
        {
	      message[0]='\0';
        }
        //sending the new balance
        memcpy(content,&message,strlen(message)+1);
	memset(&message,0,sizeof(message));
	sem_post(answer);          
   }
   
   sem_close(mutex);
   sem_unlink(MUTEX_NAME);

   sem_close(answer);
   sem_unlink(ANSWER_NAME);

   //after the server is down, we dont need client semaphore 
   sem_close(clients);
   sem_unlink(CLIENTS_NAME);
   
   //closing and destroying the shared segment
   shmdt(content);
   shmctl(shmid,IPC_RMID,NULL);
}


void sigEndHandler(const int sig_num)
{
     printf("The program will exit. The signal %d has been called\n",sig_num);
     sem_close(mutex);
     sem_unlink(MUTEX_NAME);
     sem_close(answer);
     sem_unlink(ANSWER_NAME);
     sem_close(clients);
     sem_unlink(CLIENTS_NAME);
     shmdt(content);
     shmctl(shmid,IPC_RMID,NULL);
     _exit(16);  
} 



uint32_t GetBalance(const int index,const char* const database)
{
    int fd=0;
    if((fd=open(database,O_RDONLY))==-1)
    {
      const int er=errno;
    	sem_close(mutex);
      sem_unlink(MUTEX_NAME);

      sem_close(answer);
      sem_unlink(ANSWER_NAME);

      sem_close(clients);
      sem_unlink(CLIENTS_NAME);
      
      shmdt(content);
      shmctl(shmid,IPC_RMID,NULL);
      errno=er;
    	err(10,"Couldnt open the database");
    }
    
    //postioning the pointer to the correct account
    lseek(fd,4*index,SEEK_SET);
    unsigned char value[5];
    if(read(fd,&value,sizeof(unsigned char)*4)<=0)
    {
      const int er=errno;
      sem_close(mutex);
      sem_unlink(MUTEX_NAME);

      sem_close(answer);
      sem_unlink(ANSWER_NAME);

      sem_close(clients);
      sem_unlink(CLIENTS_NAME);
      
      shmdt(content);
      shmctl(shmid,IPC_RMID,NULL);
      errno=er;
      err(11,"Couldnt get the value from the DB");
    }
    value[4]='\0';
    close(fd);
    uint32_t balance=value[0]+value[1]*16*16+value[2]*16*16*16*16+value[3]*16*16*16*16*16*16;
	  return balance;
}

unsigned char * ConvertDecimalToHex(uint32_t value)
{
    unsigned char * num =(unsigned char*)malloc(sizeof(unsigned char)*4+1);
    num[5]='\0';
    int i=0;
    while(i<4)
    {
       num[i]=value%16;
       value/=16;
       num[i]+=(value%16)*16;
       value/=16;
       i++;
    }
    return num;
}

//maybe it should be bool
bool Transaction(const int index,const int16_t val,const char* const database,uint32_t * balance)
{
    //Getting the current balance
    long bal=(long)GetBalance(index,database);
    bal+= val;
    if(bal >=0||bal<=UINT32_MAX)
    {
        int fd=0;
        if((fd=open(database,O_WRONLY))==-1)
        {
          const int er=errno;
          sem_close(mutex);
          sem_unlink(MUTEX_NAME);

          sem_close(answer);
          sem_unlink(ANSWER_NAME);

          sem_close(clients);
          sem_unlink(CLIENTS_NAME);
      
          shmdt(content);
          shmctl(shmid,IPC_RMID,NULL);
          errno=er;
    	  err(10,"Couldnt open the database");
        }
        unsigned char * hex=ConvertDecimalToHex(bal);
        lseek(fd,4*index,SEEK_SET);
        if(write(fd,hex,4)!=4)
        {
           const int er=errno;
          sem_close(mutex);
          sem_unlink(MUTEX_NAME);

          sem_close(answer);
          sem_unlink(ANSWER_NAME);

          sem_close(clients);
          sem_unlink(CLIENTS_NAME);
      
          shmdt(content);
          shmctl(shmid,IPC_RMID,NULL);
          errno=er;
          err(11,"Couldnt update the database with the new value");
        }
        (*balance)=(uint32_t)bal;
        close(fd);
        return 0;
    }
    return 1;
}

void CreateDB(const char *const DB)
{
    int fd=0;
    if((fd=open(DB,O_WRONLY|O_CREAT|O_TRUNC,0777))==-1)
    {

      err(10,"Couldnt create a database");   
    }
    const uint32_t temp=0;
    for(int i=0;i<8;i++)
    {
        if(write(fd,&temp,sizeof(temp))!=4)
        {
            const int savederrno=errno;
            close(fd);
            errno=savederrno;
            err(11,"Problem with creating the content of the file");
        }
    }
    close(fd);
}

bool CheckFormat(const char* const DB)
{ 
   int fd=0;
   if((fd=open(DB,O_RDONLY))==-1)
   {
     err(14,"Couldnt open the file");
   }
   uint32_t temp=0;
   ssize_t readsize=0;
   int counter=0;
   while((readsize=read(fd,&temp,sizeof(temp)))>0)
   {
      if(readsize!=4)
      {
        return 0;
      }
      counter++;
   }

   if(readsize==-1)
   {
     const int er=errno;
     close(fd);
     errno=er;
     err(20,"Ops, error during the check of the format of the file");
   }
   else if(counter!=8)
   {
      return 0;
   }
   return 1;
}
