#include <os.h>


#ifdef __cplusplus
extern "C" {
#endif

/* 
 * La fonction memcpy permet de copier n octets de src vers dest.
 * Les adresses sont lineaires.
 */
void* memcpy(char* dest, const char* src, int n) {
  char *p = dest;
  while (n--)
    *dest++ = *src++;
  return p;
}

/*
 * Met un ensemble memoire (dest>>n) à la valeur src
 */
void* memset(char* dest, char src, int n) {
  char *p = dest;
  while (n--)
    *dest++ = src;
  return p;
}

#ifdef __cplusplus
}
#endif
