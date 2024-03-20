#include <limits.h>
#include <stddef.h>
void* calloc(size_t nmemb, size_t size);

unsigned str_len(char* x) {
  unsigned i = 0;
  for(; x[i]; i++)
    ;
  return i;
}

unsigned strn_len(char* x, int j) {
  unsigned i = 0;
  if(j <= 0) {
    j = INT_MAX;
  }
  for(; x[i] && i < j; i++) {
  }
  return i;
}

unsigned str_cmp(char* x, char* y) {
  int i = 0;
  for(; x[i] && y[i]; i++) {
    if(x[i] != y[i]) {
      return 0;
    }
  };
  return 1;
}

unsigned strn_cmp(char* x, char* y, int j) {
  int i = 0;
  if(j <= 0) {
    j = INT_MAX;
  }
  for(; x[i] && y[i] && i < j; i++) {
    if(x[i] != y[i]) {
      return 0;
    }
  };
  return 1;
}

char* str_cpy(char* x, char* y) {
  int i = 0;
  for(; y[i]; i++) {
    x[i] = y[i];
  };
  return x;
}

char* strn_cpy(char* x, char* y, int j) {
  int i = 0;
  if(j <= 0) {
    j = INT_MAX;
  }
  for(; y[i] && i < j; i++) {
    x[i] = y[i];
  };
  return x;
}

char* str_dup(char* x) {
  char* y = calloc(1, str_len(x));
  return str_cpy(y, x);
}

char* str_cat(char* x, char* y) {
  return str_cpy(x + str_len(x), y);
}
