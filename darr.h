#ifndef DARR_H
#define DARR_H

#include <stdarg.h>
#include <stdlib.h>

typedef struct darr_type {
  void** items;
  unsigned count;
  int capacity;
  int (*append_func)(struct darr_type darr, void*);
} darr;

unsigned darr_append(darr* darr, void* data);
unsigned darr_delete(darr* darr, unsigned count, unsigned arr[]) __attribute__((nonnull(1, 3)));

#endif
