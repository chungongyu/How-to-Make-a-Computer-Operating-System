#ifndef ALLOC_H
#define ALLOC_H


#ifdef __cplusplus
extern "C" {
#endif

void *ksbrk(int);
void *kmalloc(unsigned long);
void kfree(void *);

#ifdef __cplusplus
}
#endif

#endif
