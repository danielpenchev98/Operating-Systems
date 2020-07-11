#include "FileOperations.h"

//Creating a new configuration file
void CreateConfigFile(const int argc,char * const argv[])
{
    //there should be always a pair of number and letter(type)
    if((argc-3)%2!=0)
    {
       errx(8,"Invalid format of the sequence of arguments. Cannot create the conf file");
    }

    const int numPairs=(argc-3)/2;
    struct Pair arr[numPairs];
    int cnt=0;
    for(int i=0;i<argc-4;i+=2)
    {
    	if(!CheckIfNumber(argv[i+3]))
    	{
           errx(9,"Invalid position of segment is chosen");
    	}
        arr[cnt].place=atoi(argv[i+3]);
        arr[cnt].type= strcmp(argv[i+4],"t")==0 ? 0 : (strcmp(argv[i+4],"n")==0 ? 1 : (strcmp(argv[i+4],"b")==0 ? 2 : -1)) ;
        cnt++;
    } 
    
    //sorting by positon of the sectors
    struct Pair curr;
    for(int i=1;i<numPairs;i++)
    {
       	int j=0;
        curr=arr[i];
        for(j=i-1;j>=0&&arr[j].place>curr.place;j--)
        {
           	arr[j+1]=arr[j];
        }
        arr[j+1]=curr;
    }
    
    //checking for a whole in the segment order
    for(int i=0;i<numPairs;i++)
    {
      curr=arr[i];
      if(curr.place!=i)
      {

        errx(9,"Invalid positions of segments. File wont be created");
      }
    }

    int fd=open(argv[1],O_WRONLY|O_CREAT,0777);
    if(fd==-1)
    {
        errx(3,"Couldnt create a new file");
    }

    //writing the content of the file in little endian format
    for(int i=0;i<numPairs;i++)
    {
        if(arr[i].type==-1)
        {
            errx(10,"The file contains unsupported type of segment");
        }
        write(fd,&(arr[i].type),1);
        char wr=0;
        for(int i=0;i<63;i++)
        {
            write(fd,&wr,1);
        }
    }
}

//loading the content of the bin file in an array of structures
void UploadFile(const int fd,struct Segment curr[])
{
    int counter=0;
    char buff[SEGMENT_LENGTH];
    ssize_t readsize=0;
    while((readsize=read(fd,&buff,sizeof(buff)))>0)
    {
      //if the number of read bytes isnt equal to the size of a segment - 64 bytes 
    	if(readsize!=SEGMENT_LENGTH)
    	{
           errx(12,"Error encountered during reading of the file - invalid format of file");
    	}
      //if the segment to be read isnt following the configuration or the number of segments read is greater than the segments in the required confinguration
    	else if(buff[0]!=typeSegments[counter]||counter==NUMBER_OF_SEGMENTS)
    	{
    		errx(12,"Invalid format of file");
    	}
        //Getting type of the segment
        curr[counter].type=buff[0];
        //Getting the bit section of the segment 
        for(int i=1;i<META_INFO_LENGTH;i++)
        {
          curr[counter].arguments[i-1]=buff[i];
        }
        //Getting the value section of the segment
        for(int i=META_INFO_LENGTH;i<SEGMENT_LENGTH;i++)
        {
            curr[counter].info[i-META_INFO_LENGTH]=buff[i];
        }
        counter++;
    }
    //If the number of read segments is less than the number of required or exceeds the max number of segments in a conf file - 64
    if(counter!=NUMBER_OF_SEGMENTS||counter>64)
    {
    	errx(12,"Invalid format of file.");
    }
}

//function for updating the content of the binary file
void UpdateFile(const int fd,const struct Segment content[],const int size)
{
	lseek(fd,0,SEEK_SET);
	for(int i=0;i<size;i++)
	{
       		//Writing the type of the segment
        	write(fd,&(content[i].type),1);
        	//Writing the bit section of the segment
        	write(fd,&(content[i].arguments),BITS_META_INFO_LENGTH);
        	//Writing the value section of the segment
        	write(fd,&(content[i].info),VALUE_INFO_LENGTH);
	}
}




