#ifndef HELP_FUNCTIONS_H
#define HELP_FUNCTIONS_H

#include "Constants.h"
#include <stdio.h>
#include "ContentManipulation.h"
#include <stdlib.h>
#include <regex.h>
#include <err.h>
#include <stdbool.h>

//required in order to sort the segments during the creating of a new config file
struct Pair
{
  int place;
  char type;
};

//tool help
void HelpFunction();

bool CheckEndian(); // - checking the architecture, the program is working on
unsigned char * ConvertDecimalToHex(unsigned int value);
unsigned int ConvertHexToDecimal(const unsigned char * value);
bool Regex(const char * const param,const int index);
bool CheckIfNumber(const char* const val);

#endif