#ifndef LOGGER_H
#define LOGGER_H
#include "common_headers.h"

typedef enum {
  INFO,
  DEBUG_MODE, // called DEBUG_MODE so that you can use ifdef DEBUG ...
  WARNING,
  ERROR,
} logger_enum;

typedef struct {
  char* color;
  int hide_msg;
  int debug_mode;
  int new_line;
  logger_enum logger_enum;
} logger_internal;

const char* const color[] = {
    "\033[38;5;208m[INFO]\033[0m",
    "\033[38;5;241m[DEBUG]\033[0m",
    "\033[38;5;1m[WARN]\033[0m",
    "\033[38;5;196m[ERROR]\033[0m",
};

logger_internal logger_info = {0};

void logger(char* fmt, va_list args) {
  if(logger_info.hide_msg) {
    return;
  }
  va_list l_args;
  va_copy(l_args, args);
  switch(logger_info.logger_enum) {
  case INFO:
    fprintf(stderr, "%s ", color[INFO]);
    break;
  case DEBUG_MODE:
    fprintf(stderr, "%s ", color[DEBUG_MODE]);
    break;
  case WARNING:
    fprintf(stderr, "%s ", color[WARNING]);
    break;
  case ERROR:
    fprintf(stderr, "%s ", color[ERROR]);
    break;
  }
  vfprintf(stderr, fmt, l_args);
  if(logger_info.new_line) {
    fprintf(stderr, "\n");
  }
}

void debug_log(char* fmt, ...) {
#ifndef DEBUG
  return;
#endif
  logger_info.logger_enum = DEBUG_MODE;
  logger_info.new_line = 1;
  va_list args;
  va_start(args, fmt);
  logger(fmt, args);
  va_end(args);
  return;
}

#endif
