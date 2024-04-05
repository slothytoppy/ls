#ifndef DARR_H
#define DARR_H

typedef struct darr_type {
  void** items;
  unsigned count;
  int capacity;
  int (*append_func)(struct darr_type darr, void*);
} darr;

unsigned append(darr* darr, void* data, ...);
unsigned darr_delete(darr* darr, int arr[]);

#endif
