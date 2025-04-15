#ifndef ERROR_DEMO_H_
#define ERROR_DEMO_H_

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

extern int errno;
extern const char * const sys_errlist[];
extern const int sys_nerr;

#endif