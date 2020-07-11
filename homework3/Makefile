ifndef CC
	CC=gcc
endif
CFLAGS=-std=c99 -Wall -Wpedantic -Wextra
SRCS=main.c Constants.c FileOperations.c HelpFunctions.c ContentManipulation.c
OBJS=$(subst .c,.o,$(SRCS))
RM=rm -f

all:	
	$(CC) $(CFLAGS) -o main $(SRCS)
clean:
	$(RM) $(OBJS) main

