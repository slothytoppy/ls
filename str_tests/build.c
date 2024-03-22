#include "../nom/nom.h"

int main(int argc, char** argv) {
  for(int i = 0; i < argc; i++) {
    printf("%s:%d ", argv[i], i);
  }
  if(rebuild1(__FILE__, "gcc")) { // without this it runs everything twice
    return 0;
  }
  char* library = calloc(1, 255);
  char* buff = calloc(1, 4096);
  char* cwd = getcwd(buff, 4096);
  char* base_cwd = strrchr(cwd, '/');
  base_cwd += 1;
  Nom_cmd cmd = {0};
  if(strcmp(base_cwd, "str_tests") == 0) {
    nom_log(NOM_PANIC, "base cwd: %s", base_cwd);
    library = "../str_utils.c";
    if(!IS_PATH_EXIST(library)) {
      nom_log(NOM_PANIC, "library: %s does not exist", library);
    }
  } else {
    library = "str_utils.c";
  }
  nom_cmd_append_many(&cmd, 5, "gcc", "t1.c", library, "-o", "t1");
  nom_run_sync(cmd);
  nom_cmd_reset(&cmd);
  nom_cmd_append(&cmd, "./t1");

  if(!nom_run_path(cmd, NULL)) {
    return 1;
  }
}
