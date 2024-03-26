#include "../str_utils.h"
#include "../test.h"
#include <stdio.h>

int main(void) {
  if(!ret_eq(ends_with("hello.c", ".c"), 1)) {
    printf("ends_with failed with return code 1\n");
    return 1;
  }
  if(!ret_eq(str_len("hello"), 5)) {
    printf("str_len failed to return accurate length for string: 'hello'\n");
    return 1;
  }
  if(!ret_eq(strn_len("hello.c", 5), 5)) {
    printf("strn_len failed to return accurate length for string: 'hello'\n");
    return 1;
  }
  if(!ret_eq(str_cmp("h", "h"), 1)) {
    printf("str_cmp failed to compare the same character\n");
    return 1;
  }
  if(!ret_eq(strn_cmp("he", "h", 1), 1)) {
    printf("strn_cmp failed to compare the same character\n");
    return 1;
  }
}
