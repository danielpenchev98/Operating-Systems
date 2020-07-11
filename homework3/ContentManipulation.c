#include "ContentManipulation.h"

//function for checking if the value of the 3rd element is a valid name of a parameter
//we need "index" in order to get the position of the name of the parameter in the array "paramName" 
struct PositionSegNum GetArgPosition(const char * const param,int * const index)
{
	struct PositionSegNum temp;
	if(strcmp(param,paramName[0])==0)
    {
   	  	temp.position=DEVICE_NAME;
   	  	temp.segnum=DEVICE_NAME_SEG;
   	  	temp.valPos=DEVICE_NAME_VALUE_POS;
   	  	*(index)=0;
   	}
    else if(strcmp(param,paramName[1])==0)
    {
        temp.position=ROM_REVISION;
   	  	temp.segnum=ROM_REVISION_SEG;
   	  	temp.valPos=ROM_REVISION_VALUE_POS;
   	  	*(index)=1;
    }
    else if(strcmp(param,paramName[2])==0)
    {
    	temp.position=SERIAL_NUMBER;
    	temp.segnum=SERIAL_NUMBER_SEG;
    	temp.valPos=SERIAL_NUMBER_VALUE_POS;
    	*(index)=2;
    }
    else if(strcmp(param,paramName[3])==0)
    {
        temp.position=BD_ADDR_PART0;
   	  	temp.segnum=BD_ADDR_PART0_SEG;
   	  	temp.valPos=BD_ADDR_PART0_VALUE_POS;
   	  	*(index)=3;
    }
    else if(strcmp(param,paramName[4])==0)
    {
        temp.position=BD_ADDR_PART1;
   	  	temp.segnum=BD_ADDR_PART1_SEG;
   	  	temp.valPos=BD_ADDR_PART1_VALUE_POS;
   	  	*(index)=4;
    }
    else if(strcmp(param,paramName[5])==0)
    {
        temp.position=BD_PASS_PART0;
   	  	temp.segnum=BD_PASS_PART0_SEG;
   	  	temp.valPos=BD_PASS_PART0_VALUE_POS;
   	  	*(index)=5;
    }
    else if(strcmp(param,paramName[6])==0)
    {
        temp.position=SERIAL_BAUDRATE;
   	  	temp.segnum=SERIAL_BAUDRATE_SEG;
   	  	temp.valPos=SERIAL_BAUDRATE_VALUE_POS;
   	  	*(index)=6;
    }
    else if(strcmp(param,paramName[7])==0)
    {
        temp.position=AUDIO_BITRATE;
   	  	temp.segnum=AUDIO_BITRATE_SEG;
   	  	temp.valPos=AUDIO_BITRATE_VALUE_POS;
   	  	*(index)=7;
    }
    else if(strcmp(param,paramName[8])==0)
    {
        temp.position=SLEEP_PERIOD;
   	  	temp.segnum=SLEEP_PERIOD_SEG;
   	  	temp.valPos=SLEEP_PERIOD_VALUE_POS;
   	  	*(index)=8;
    }
    else if(strcmp(param,paramName[9])==0)
    {
        temp.position=SERIAL_PARITY;
   	  	temp.segnum=SERIAL_PARITY_SEG;
   	  	temp.valPos=SERIAL_PARITY_VALUE_POS;
   	  	*(index)=9;
    }
    else if(strcmp(param,paramName[10])==0)
    {
        temp.position=SERIAL_DATA_BIT;
   	  	temp.segnum=SERIAL_DATA_BIT_SEG;
   	  	temp.valPos=SERIAL_DATA_BIT_VALUE_POS;
   	  	*(index)=10;
    }
    else if(strcmp(param,paramName[11])==0)
    {
        temp.position=SERIAL_STOP_BIT;
   	  	temp.segnum=SERIAL_STOP_BIT_SEG;
   	  	temp.valPos=SERIAL_STOP_BIT_VALUE_POS;
   	  	*(index)=11;
    }
    else if(strcmp(param,paramName[12])==0)
    {
        temp.position=BD_PASS_PART1;
   	  	temp.segnum=BD_PASS_PART1_SEG;
   	  	temp.valPos=BD_PASS_PART1_VALUE_POS;
   	  	*(index)=12;
    }
    else if(strcmp(param,paramName[13])==0)
    {
        temp.position=ROM_CHECKSUM_PART0;
   	  	temp.segnum=ROM_CHECKSUM_PART0_SEG;
   	  	temp.valPos=ROM_CHECKSUM_PART0_VALUE_POS;
   	  	*(index)=13;
    }
    else if(strcmp(param,paramName[14])==0)
    {
        temp.position=ROM_CHECKSUM_PART1;
   	  	temp.segnum=ROM_CHECKSUM_PART1_SEG;
   	  	temp.valPos=ROM_CHECKSUM_PART1_VALUE_POS;
   	  	*(index)=14;
    }
    else
    {
       errx(14,"Invalid name of parameter");
    }
    return temp;
}

void SetBit(struct Segment content[],const struct PositionSegNum curr,const bool value)
{
   //getting the position of the byte containing the wanted bit 
   int numByte=curr.position/8;
   
   //getting the position of the bit in the byte containing the wanted bit
   int offset=curr.position%8;
   if(value==1)
   {    
        content[curr.segnum].arguments[numByte] |= (1<<(7-offset));
   }
   else
   {
   	   content[curr.segnum].arguments[numByte] &= ~(1<<(7-offset));
   }
}

bool GetBit(const struct Segment content[],const struct PositionSegNum curr)
{
	//getting the position of the byte containing the wanted bit 
	int numByte=curr.position/8;
	//getting the position of the bit in the byte containing the wanted bit
	int offset=curr.position%8;
    return content[curr.segnum].arguments[numByte] & (1<<(7-offset));
}
void SetValue(struct Segment content[],const struct PositionSegNum curr ,const char * const value)
{
	int i=0;
    int cnt=0;
	if(content[curr.segnum].type==1)
	{
	   unsigned char * num=ConvertDecimalToHex(atoi(value));
	   while(i<NUMERIC_DATA_TYPE)
	   {
	   	    content[curr.segnum].info[NUMERIC_DATA_TYPE*curr.valPos+cnt]=num[i++];
	   	    cnt++;
	   }
	   free(num);
	}
	else if(content[curr.segnum].type==2)
	{
		//for example 1 , 10, 99
        if(value[0]>=48&&value[0]<=57&&value[1]=='\0')
        {
            content[curr.segnum].info[BYTE_DATA_TYPE*curr.valPos+cnt]=(value[0]-'0');
        }
        // example A-Z, a-z...
        else if(value[1]=='\0')
        {
            content[curr.segnum].info[BYTE_DATA_TYPE*curr.valPos+cnt]=value[0];
        }
        else if(strcmp(value,"")==0)
        {
            content[curr.segnum].info[BYTE_DATA_TYPE*curr.valPos+cnt]=0;
        }
        else
        {
        	errx(16,"Critical logical error.");
        }
	}
	else
	{
		while(value[i]!='\0'&&i<TEXT_DATA_TYPE)
    	{
       		content[curr.segnum].info[TEXT_DATA_TYPE*curr.valPos+cnt]=value[i];
       		cnt++;
       		i++;
    	}

    	//if the value is more than 16 bytes
    	if(i==TEXT_DATA_TYPE&&value[i]!='\0')
    	{
    		errx(15,"Invalid length of value");
    	}

    	while(cnt!=TEXT_DATA_TYPE)
    	{
       		content[curr.segnum].info[TEXT_DATA_TYPE*curr.valPos+cnt]=0;
       		cnt++;
    	}
	}
}

void PrintValue(const struct Segment content[],const struct PositionSegNum curr)
{
    int cnt=0;
	if(content[curr.segnum].type==2)
	{
       unsigned char val=content[curr.segnum].info[BYTE_DATA_TYPE*curr.valPos];
       if(val<=9)
       {
       	  printf("%d",val);
       }
       else
       {
       	  printf("%c",val);
       }
	}
	else if(content[curr.segnum].type==1)
	{
	   unsigned char * value=(unsigned char*)malloc(sizeof(unsigned char)*NUMERIC_DATA_TYPE+1);
	   value[NUMERIC_DATA_TYPE]='\0';
	   int i=0;
       while(i<NUMERIC_DATA_TYPE)
       {
           value[i++]=content[curr.segnum].info[NUMERIC_DATA_TYPE*curr.valPos+cnt];     
           cnt++;
       }
       unsigned int num=ConvertHexToDecimal(value);
       free(value);
       printf("%d",num);
	}
	else
	{
		while(cnt<TEXT_DATA_TYPE&&content[curr.segnum].info[TEXT_DATA_TYPE*curr.valPos+cnt]!=0)
    {
       	printf("%c",content[curr.segnum].info[TEXT_DATA_TYPE*curr.valPos+cnt]);
       	cnt++;
 
    }
  }
}

