#include "../str_utils.h"
#include "../test.h"
#include <stdio.h>

int main(void) {
  if(!ret_eq(ends_with("hello.c", ".c"), 1)) {
    return 1;
  }
  if(!ret_eq(str_len("hello"), 5)) {
    return 1;
  }
  if(!ret_eq(strn_len("hello.c", 5), 5)) {
    return 1;
  }
  if(!ret_eq(str_cmp("h", "h"), 1)) {
    return 1;
  }
  if(!ret_eq(strn_cmp("he", "h", 1), 1)) {
    return 1;
  }
}
