#include "nom/nom.h"

int main(void) {
  char* argv[] = {base(__FILE__), NULL};
  rebuild(1, argv, __FILE__, "gcc");
  Nom_cmd cmd = {0};
  nom_cmd_append_many(&cmd, 4, __FILE__, "str_utils.c", "-o", "main");
  nom_run_sync(cmd);
  return 0;
}
