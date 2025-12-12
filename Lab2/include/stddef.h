#ifndef STDDEF_H
#define STDDEF_H

typedef unsigned int size_t;
#define NULL ((void *)0)

int memcmp(const void *s1, const void *s2, size_t n);
#endif