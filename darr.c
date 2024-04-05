#include "./darr.h"

int printf(const char*, ...);

unsigned append(darr* darr, void* data) {
  if(darr->count == 0) {
    darr->items = calloc(1, 8 * sizeof(char*));
    darr->capacity = 8;
  } else {
    darr->items = realloc(darr->items, (darr->count + 3) * sizeof(void*));
    if(darr->count >= darr->capacity) {
      darr->capacity *= 2;
      darr->items = realloc(darr->items, darr->capacity * sizeof(void*));
    }
  }
  if(darr->items == NULL) {
    return 0;
  }
  darr->items[darr->count] = data;
  darr->count += 1;
  darr->items[darr->count] = NULL;
  return 1;
}

unsigned darr_delete(darr* darr, unsigned count, unsigned arr[]) {
  if(darr->count == 0) {
    return 0;
  }
  for(unsigned i = 0; i < count; i++) {
    if(arr[i] > darr->count) {
      printf("index: %d is greater than darr->count: %d\n", arr[i], darr->count);
      return 0;
    }
    darr->items[arr[i]] = NULL;
  }
  return 1;
}
