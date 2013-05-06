#include <stdio.h>
#ifndef _WIN32
	#include "stdio_ext.h"
    #define  fflush __fpurge
#endif
#ifndef OPTLIB_H
int getopt_c (char*,int);
int get_posint(int*,FILE*);
char fgets_c(char*,int,FILE*);
char* fget_str(FILE*,int,char,int,...);
#endif