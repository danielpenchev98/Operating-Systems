#ifndef CONTENT_MANIPULATION_H
#define CONTENT_MANIPULATION_H

#include <stdlib.h>
#include <stdio.h>
#include "HelpFunctions.h"
#include <err.h>
#include <string.h>
#include <stdbool.h>


//struct for getting the position of the bit's place corresponding to a specific parameter, also the number of segment, in which it is used, and the mapping between the parameter place in the bit list and its value in the main section of the segment
struct PositionSegNum
{
	int position;
	int segnum;
	int valPos;
};

//Structute uploading a segment
struct Segment
{
    char type;
    char arguments[BITS_META_INFO_LENGTH];
    char info[VALUE_INFO_LENGTH];
};

//Operations for manipulating the uploaded file content
struct PositionSegNum GetArgPosition(const char * const param,int * const index);
void SetBit(struct Segment content[],const struct PositionSegNum curr,const bool value);
bool GetBit(const struct Segment content[],const struct PositionSegNum curr);
void SetValue(struct Segment content[],const struct PositionSegNum curr,const char * const value);
void PrintValue(const struct Segment content[],const struct PositionSegNum curr);
#endif