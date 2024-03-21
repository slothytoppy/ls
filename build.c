#include "nom/nom.h"

int main(int argc, char* argv[]) {
  char* program_name = "main.c";
  rebuild1(__FILE__, "gcc");
  Nom_cmd cmd = {0};
  nom_cmd_append_many(&cmd, 5, "gcc", program_name, "str_utils.c", "-o", base(program_name));
  nom_run_sync(cmd);
  nom_cmd_reset(&cmd);
  nom_cmd_append_many(&cmd, 1, base(program_name));
  nom_run_path(cmd, argv);
  return 0;
}
