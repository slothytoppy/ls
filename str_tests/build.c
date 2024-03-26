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
  char* file = calloc(1, strlen("t1.c"));
  base_cwd += 1;
  Nom_cmd cmd = {0};
  Nom_cmd run = {0};
  if(strcmp(base_cwd, "str_tests") == 0) {
    library = "../str_utils.c";
    file = "t1.c";
    if(!IS_PATH_EXIST(library)) {
      nom_log(NOM_PANIC, "library: %s does not exist", library);
    }
    nom_cmd_append(&run, "./t1");
  } else {
    library = "str_utils.c";
    file = realloc(file, strlen(base_cwd) + strlen("t1.c"));
    file = "./str_tests/t1.c";
    nom_cmd_append(&run, base(file));
  }
  nom_cmd_append_many(&cmd, 5, "gcc", file, library, "-o", base(file));
  nom_run_sync(cmd);
  if(!nom_run_path(run, NULL)) {
    return 1;
  }
}
