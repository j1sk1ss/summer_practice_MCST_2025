#ifndef ERROR_DEMO_H_
#define ERROR_DEMO_H_

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define ERRLIST_OPTION  "--errlist"
#define PERROR_OPTION   "--perror"
#define ERRNO_OPTION    "--errno"

extern int errno;
extern const char * const sys_errlist[];
extern const int sys_nerr;

#endif