#ifndef ERROR_DEMO_H_
#define ERROR_DEMO_H_

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define HELP_OPTION     "--help"
#define ERRLIST_OPTION  "--errlist"
#define PERROR_OPTION   "--perror"
#define ERRNO_OPTION    "--errno"

#if defined(__GLIBC__) && (__GLIBC__ < 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ < 32))
    extern const int sys_nerr;
    extern const char *const sys_errlist[];
#endif

extern int errno;

#endif