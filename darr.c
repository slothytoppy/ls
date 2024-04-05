#include <stdlib.h>

#include "./darr.h"
#include <stdarg.h>

unsigned append(darr* darr, void* data) {
  if(darr->append_func) {
    if(darr->append_func(*darr, data)) {
      return 1;
    } else {
      return 0;
    }
  }
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
