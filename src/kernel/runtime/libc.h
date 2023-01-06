#ifndef LIBC_H
#define LIBC_H

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

void    itoa(char* buf, unsigned long int n, int base);

void*   memset(char* dest, char c, int n);
void*   memcpy(char* dest, const char* src, int n);

int     strlen(const char* s);
int     strcmp(const char* s1, const char* s2);
int     strcpy(char* dest, const char* src);
char*   strcat(char* dest,const char* src);
char*   strncpy(char* dest, const char* src, int n);
int     strncmp(const char* s1, const char* s2, int n);

#ifdef __cplusplus
}
#endif

#endif  // LIBC_H
