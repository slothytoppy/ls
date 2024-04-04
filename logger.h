#ifndef COMMON_HEADERS_H
#define COMMON_HEADERS_H
#include "common_headers.h"

typedef struct {
  char* fmt;
  char* color;
  int hide_msg;
  enum {
    INFO,
    DEBUG,
    WARNING,
    ERROR,
    PANIC,
  };
} logger_info;

#endif
