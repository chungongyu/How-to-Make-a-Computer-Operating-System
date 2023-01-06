#include <os.h>


#ifdef __cplusplus
extern "C" {
#endif

int strlen(const char* s) {
  int i = 0;
  while (*s++)
    ++i;
  return i;
}

char* strncpy(char* dest, const char* src, int n) {
  if (dest == NULL || src == NULL) {
    return (dest = NULL);
  }

  int i = 0;
  for (i = 0; i < n && src[i] != '\0'; ++i) {
    dest[i] = src[i];
  }
  for (i = 0; i < n; ++i) {
    dest[i] = '\0';
  }

  return dest;
}

int strcmp(const char* s1, const char* s2) {
  int result = 0;

  while (true) {
    result = *s1 - *s2++;

    if (result != 0 || *s1++ == '\0' ) {
      break;
    }
  }

  return result;
}

int strcpy(char* dest, const char* src) {
  int n = 0;
  while ((dest[n] = src[n++]));
  return n;
}

char* strcat(char *dest, const char* src) {
  int n1 = strlen(dest), n2 = strlen(src);
  memcpy(dest + n1, src, n2);
  dest[n1 + n2] = '\0';
  return dest;
}

int strncmp( const char* s1, const char* s2, int c ) {
  int result = 0;

  while (c > 0) {
    result = *s1 - *s2++;

    if (result != 0 || *s1++ == '\0' ) {
      break;
    }

    --c;
  }

  return result;
}

#ifdef __cplusplus
}
#endif
