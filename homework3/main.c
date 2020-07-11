#include <sys/stat.h>

//user defined headers
#include "Constants.h"
#include "ContentManipulation.h"
#include "FileOperations.h"
#include "HelpFunctions.h"

int main(int argc,char * argv[])
{
    if(argc<2)
    {
    	errx(1,"Invalid number of arguments, no option has been chosen");
    }

    if(strcmp(argv[1],"-h")==0)
    {
      if(argc!=2)
      {
         errx(1,"\"-h\" doesnt accept any arguments");
      }
      HelpFunction();
    	exit(0);
    }
    else if(argc<3)
    {
    	errx(1,"Invalid number of arguments. The only option that doesnt accept arguments is is \"-h\"");
    }

    //chech if the first param  is a regular file  
    struct stat * buffer=malloc(sizeof(struct stat));
    stat(argv[1],buffer);
    if(!S_ISREG(buffer->st_mode)&&strcmp(argv[2],"-c")==0)
    {
      CreateConfigFile(argc,argv);
      exit(0);
    }
    //If a file with that name exists, and the selected option is "-c" then the program will show an error and exit.
    else if(strcmp(argv[2],"-c")==0)
    {
    	errx(2,"A file with that name already exists. Cannot use -c with an existing name of a file ");
    }
    free(buffer);

    //struct for uploading the binary file content
    struct Segment content[NUMBER_OF_SEGMENTS];
      
    int fd=0;
    if((fd=open(argv[1],O_RDWR))==-1)
    {
    	errx(3,"Couldnt open the bin file");
    }
    else
    {
        UploadFile(fd,content);
        //closing the file, because we dont actually need it anymore
        close(fd);
    }
     
    int index=0;
    struct PositionSegNum pos;
    if(strcmp(argv[2],"-s")==0||strcmp(argv[2],"-S")==0)
    {
      if(argc!=5)
      {
      	errx(1,"Invalid number of arguments for the option \"-s/-S\"");
      }

     //HOW TO DELETE A VALUE - JUST USE "" as value to be set as a value on that particular parameter
      pos=GetArgPosition(argv[3],&index);
      if(pos.segnum==-1)
    	{
    		  errx(5,"Sorry but this parameter isnt attached to any of the used segments");
    	}
      else if(strcmp(argv[4],"")!=0&&!Regex(argv[4],index))
      {
          errx(4,"Invalid value of argument to be assigned to that parameter");
      }
      else if(strcmp(argv[2],"-s")==0)
      {
          SetBit(content,pos,1);
      }
      SetValue(content,pos,argv[4]);
    
    }
    else if(strcmp(argv[2],"-g")==0||strcmp(argv[2],"-G")==0)
    {
    	if(argc!=4)
    	{
    		errx(1,"Invalid number of arguments for option \"-g/-G\"");
    	}

    	pos=GetArgPosition(argv[3],&index);
    	if(pos.segnum==-1)
    	{
    		errx(5,"Sorry but this parameter isnt attached to any of the used segments");
    	}
      else if(GetBit(content,pos)||strcmp(argv[2],"-G")==0)
      {
        PrintValue(content,pos);
        printf(" - %s\n",argv[3]);
      }
    
    }
    else if(strcmp(argv[2],"-l")==0||strcmp(argv[2],"-L")==0)
    {
    	int sz=0;
      if(argc==3)
    	{ 
    		 sz=paramSize;
    	}
    	else
    	{
    		 sz=argc;    
    	}
      //if argc is 3 => show all names of parameters, else show the value of the parameters listed after the 4rth element in the list
    	int i=(argc==3 ? 0 : 3);
    	for(;i<sz;i++)
      {
          pos=GetArgPosition(argc==3 ? paramName[i] : argv[i],&index);
          if(pos.segnum==-1)
    		  {
    			    errx(5,"Sorry but this parameter isnt attached to any of the used segments");
    		  }
          if(GetBit(content,pos)||strcmp(argv[2],"-L")==0)
          {
              if(argc!=3)
              {
                  PrintValue(content,pos);
                  printf(" - %s\n",argv[i]);
              }
              else if(argc==3)
              {
                  printf("%s\n",paramName[i]);
              }
          }
      }
    }
    else if(strcmp(argv[2],"-b")==0)
    {
    	 if(argc!=5)
    	 {
    	   	errx(1,"Invalid number of arguments for option \"-b\"");
    	 }

       pos=GetArgPosition(argv[3],&index);
       if(pos.segnum==-1)
    	 {
    	  	errx(5,"Sorry but this parameter isnt attached to any of the used segments");
    	 }

       printf("%d\n",index);
    	 if(argv[4][1]!='\0'||(argv[4][0]!='0'&&argv[4][0]!='1'))
    	 {
    		 close(fd);
    		 errx(6,"Invalid bool value");
    	 }
    	 int val=atoi(argv[4]);
       SetBit(content,pos,val);
    }
    else 
    {
    	close(fd);
    	errx(7,"Wrong option");
    }

    //opening again the file to update it
    if((fd=open(argv[1],O_RDWR))==-1)
    {
    	  errx(3,"Couldnt open the bin file");
    }
    else
    {   
        //updating the content of the binary file
        UpdateFile(fd,content,NUMBER_OF_SEGMENTS);
        close(fd);
    }
    exit(0);
}
