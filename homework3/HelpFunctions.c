#include "HelpFunctions.h"


void HelpFunction()
{
	  printf("main              General Commands Manual            \n\n");		
    printf("NAME\n");
    printf("	main - the Swiss Army knife for configuring a Bluetooth device\n\n");
    printf("SYNOPSIS\n");
    printf("	main [-h]\n        main [binary file] [-lL] [parameter names]{0...n}\n        main [binary file] [-gG] [parameter name]\n        main [binary file] [-b] [parameter name] [value of bit]\n        main [binary file] [-sS] [parameter name] [value]\n        main [binary file] [-c] [position of segment1] [type of segment1] ... [position of segmentN] [type of segmentN]\n\n");
    printf("DESCRIPTION\n");
    printf("	This manual page documents an instrument for configuring a bluetooth device. This instrument works\n");
    printf("	with binary files with a fixed particular format. Supports easy modification of the file format \n");
    printf("	in order to be used to configure diffrent types of bluetooth devices. There are several functions to be offered:\n");
    printf("OPTIONS\n");
    printf("       -s [parameter_name] [value] - this options changes the paramter's value in value section of the segment. If the parameter \n");
    printf("                                     was assigned to a segment and the bit responsible for the parameter wasnt activated\n");
    printf("                                     then it will be activated(bit set to 1\n");
    printf("       -S [parameter_name] [value] - same as \"-s\" but the bit responsible for the parameter doesnt matter in what condition it is\n");
    printf("                                   - Note: if the user wants to delete the value of an argument just use \"\" as an value\n");
    printf("       -g [paramater_name] - prints on the terminal the value of the parameter if it's a valid one and is activated\n");
    printf("       -G [parameter_name] - same as \"-g\" but doesnt matter if the parameter is activated or not\n");
    printf("                           - Note: both -g and -G print on the terminal in the format:[value] - [parameter name]\n");
    printf("       -l [parameter_name1]...[parameter_nameN] - if there arent any values after \"-l\" then on the terminal the names of all used(activated)\n");
    printf("                                                  parameters are printed, else the values of paramters after \"-l\" are printed\n");
    printf("       -L [parameter_name1]...[parameter_nameN] - same as \"-l\" but the program doenst check if the parameters are activated(if their bits are 1)\n");
    printf("       -b [parameter_name] [value(1 or 0)] - activates or deactivates the parameter_name's bit\n");
    printf("       -c [position1] [type1] ... [positionN] [typeN] - creates a new binary file containing  a sequence of\n");
    printf("                                                        segments -([number in the sequence-positionN] [type of segment-typeN])\n");
    printf("       -h - help page for the instrument - USE LESS to see it better + go fullscreen\n\n");
    printf("CHANGE FORMAT\n");
    printf("       - Number and types of segment\n");
    printf("                             - change the \"NUMBER_OF_SEGMENTS\" value\n");
    printf("                             - change the \"typeSegments\" array.\n\n");
    printf("       Note if your segments dont use a particular paramater, then go to Delete a parameter section\n\n");
    printf("       - Add new parameter\n");
    printf("                             - change the paramName array, add the new parameter\n");
    printf("                             - add #define constant <paramname> <position of param in segment>\n");
    printf("                             - add #define constant <paramname_SEG> <number of segment>\n");
    printf("                             - add #define constant <paramname_VALUE_POS> <position of value of param in data part of segment> ( for mapping)\n");
    printf("                             - add rules in function \"Regex\"\n");
    printf("                             - add add new if(strcmp(paramName[{the index in the array \"paramName\"}],param)==0) in \"GetArgPosition\" function  \n");
    printf("                               and assign the values of the argument's constants to the variable temp\n\n");
    printf("       - Modify a paramater\n");
    printf("                             - modify Name - change name in paramName array\n");
    printf("                             - modify Position of its bit in segment - in #define {name} val\n");
    printf("                             - modify the number of segment, in which it participates - in #define {name}_SEG val\n");
    printf("                             - modify the position of the value of the parameter - in #define {name}_VALUE_POS val\n");
    printf("       - Delete a parameter\n");
    printf("                             - remove it from lists of  \"#defines\" the \"paramName\" array \n");
    printf("                             - update the REGEX rules in \"Regex\" function\n");
    printf("                             - update \"GetArgPosition\" function \n");
    printf("                             - or skip the options above and just set the its *_SEG constant to -1\n\n");
    printf("       - Modify restrictions for the value of a parameter\n");
    printf("                             - look the index of the argument in paramName\n");
    printf("                             - change the rules in \"paramLimits\" array  - mapping is 1:1\n");
    printf("PARAMETERS\n");
    printf("                             parameter; segment; position; mappingPositionToDataSection; valid values\n\n");
    printf("                             - IGNORE the symbols \"^\" and \"$\"  and the symbol \"|\" means \"or\"\n");
    for(int i=0;i<paramSize;i++)
    {
      int inx=0;
      struct PositionSegNum pos=GetArgPosition(paramName[i],&inx);
      printf("                             %s; %d; %d; %d; %s;\n",paramName[i],pos.segnum,pos.position,pos.valPos,paramLimits[i]);
    }
    printf("SEGMENTS SEQUENCE\n");
    printf("                             id;type\n\n");
    for(int i=0;i<NUMBER_OF_SEGMENTS;i++)
    {
      printf("                              %d;%s",i,typeSegments[i]==0?"text\n":typeSegments[i]==1?"numeric\n":"byte\n");
    }
    printf("\nDATA TYPES IN TYPES OF SEGMENTS\n");
    printf("                             0; text; %d, NULL terminated\n",TEXT_DATA_TYPE);
    printf("                             1; numeric; %d byte digits\n",NUMERIC_DATA_TYPE);
    printf("                             2: byte; %d byte digits\n\n",BYTE_DATA_TYPE);   
    printf("EXIT STATUS RETURNED\n");
    printf("                             0 - everything is ok\n");
    printf("                             1 - problem with the number of arguments\n");
    printf("                             2 - The file the user wants to create already exists\n");
    printf("                             3 - Problem with opening/creating a file\n");
    printf("                             4 - The given value violates the constrains of that particular parameter\n");
    printf("                             5 - The parameter the user chose isnt assigned to any of the segments\n");
    printf("                             6 - Invalid bool value\n");
    printf("                             7 - Wrong option has been chosen\n");
    printf("                             8 - Problem with the pairs [sequnceNumber] [Type of segment]\n");
    printf("                             9 - The value given is diffrent from a number\n");
    printf("                            10 - Unsupported type of segment\n");
    printf("                            12 - Invalid format of file\n");
    printf("                            14 - Invalid name of parameter\n");
    printf("                            15 - Invalid length of value\n");
    printf("                            16 - Critical logical error(shouldnt happen at all)\n\n");
    printf("BUG REPORTING\n");
    printf("                             If any bugs are found please contact me via danielpenchev@abv.bg\n\n");
    printf("AUTHOR\n");
    printf("                             Written by Daniel Penchev\n");
}



bool CheckEndian()
{
	int  temp=0x0123456;
	char * c=(char*)&temp;
	if(*(c)>*(c+1))
	{
		return 1;
	}
	return 0;
}

//should be endian independent
//function for converting 4-byte number to char array of 4 elements, containing the hex value of the number in little endian format
unsigned char * ConvertDecimalToHex(unsigned int value)
{
    unsigned char * num =(unsigned char*)malloc(sizeof(unsigned char)*NUMERIC_DATA_TYPE+1);
    num[NUMERIC_DATA_TYPE]='\0';
    int i=0;
    while(i<NUMERIC_DATA_TYPE)
    {
       num[i]=value%16;
       value/=16;
       num[i]+=(value%16)*16;
       value/=16;
       i++;
    }
    return num;
}

//should be endian independent
//function for converting hex value saved in char array of 4 elements in little endian format to 4-bytes number
unsigned int ConvertHexToDecimal(const unsigned char * const  value)
{
	return value[0]+value[1]*16*16+value[2]*16*16*16*16+value[3]*16*16*16*16*16*16;
}

//Function for checking the values of the parameters to be set
bool Regex(const char * const value,const int index)
{
   regex_t re;
   //Creating the regex
   if(regcomp(&re,paramLimits[index],REG_EXTENDED|REG_NOSUB) != 0)
   {
     err(16,"Couldnt compile the regex");
   }
   //Checking if the value violates the constrains
   int status=regexec(&re,value,0,NULL,0);
   regfree(&re);
   if(status!=0) return 0;
   return 1;
}

bool CheckIfNumber(const char* const val)
{
	int i=0;
	while(val[i]!='\0')
	{
		if(val[i]<'0'||val[i]>'9')
		{
			return false;
		}
		i++;
	}
	return true;
}

