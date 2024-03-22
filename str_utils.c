#include <limits.h>
#include <stddef.h>
void* calloc(size_t nmemb, size_t size);
int printf(const char*, ...);

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

int ends_with(char* x, char* suff) {
  int xlen, sufflen;
  xlen = str_len(x);
  sufflen = str_len(suff);
  if(sufflen > xlen) {
    printf("sufflen>xlen\n");
    return 0;
  }
  char* xdup = x;
  char* ydup = suff;
  int start_len = xlen - sufflen;
  for(int i = 0; i < sufflen; i++) {
    if(xdup[start_len + i] != suff[i]) {
      printf("x:%c y:%c i:%d\n", xdup[start_len + i], ydup[i], i);
      return 0;
    }
  }
  printf("x:%s suff:%s\n", xdup, ydup);
  printf("real x:%s real suff:%s\n", x, suff);
  return 1;
}
