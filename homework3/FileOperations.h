#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <err.h>
#include "HelpFunctions.h"
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "ContentManipulation.h"

//Operations with binary files
void CreateConfigFile(const int argc,char * const argv[]);
void UploadFile(const int fd,struct Segment curr[]);
void UpdateFile(const int fd,const struct Segment content[],const int size);

#endif