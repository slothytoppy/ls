#include "nom/nom.h"
#include <string.h>

int build(char** files, int size) {
  if(size == 0 || files == NULL) {
    return 0;
  }
  Nom_cmd cmd = {0};
  for(int i = 0; i < size; i++) {
    nom_cmd_append_many(&cmd, 5, "gcc", files[i], "-o", base(files[i]));
    if(!nom_run_sync(cmd)) {
      nom_cmd_reset(&cmd);
      return 0;
    }
    nom_cmd_reset(&cmd);
  }
  return 1;
}

int build_with_lib(char** files, char** libraries, int file_count, int lib_count) {
  if(files == NULL || libraries == NULL || file_count == 0 || lib_count == 0) {
    return 0;
  }
  Nom_cmd cmd = {0};
  for(int i = 0; i < file_count; i++) {
    for(int j = 0; j < lib_count; j++) {
      nom_cmd_append_many(&cmd, 5, "gcc", files[i], libraries[j], "-o", base(files[i]));
      if(!nom_run_sync(cmd)) {
        nom_cmd_reset(&cmd);
        return 0;
      }
      nom_cmd_reset(&cmd);
    }
  }
  return 1;
}

int run(char* file, char** argv) {
  if(file == NULL) {
    return 0;
  }
  Nom_cmd cmd = {0};
  nom_cmd_append(&cmd, file);
  if(!nom_run_path(cmd, argv)) {
    nom_cmd_reset(&cmd);
    return 0;
  }
  nom_cmd_reset(&cmd);
  return 1;
}

int main(int argc, char* argv[]) {
  rebuild(argc, argv, __FILE__, "gcc");
  char* program_name = "main.c";
  Nom_cmd cmd = {0};
  char* files[] = {program_name};
  char* libs[] = {"./str_utils.c"};
  if(argc > 1 && strcmp(argv[1], "-d") == 0) {
    nom_cmd_append_many(&cmd, 7, "gcc", "-ggdb", "-DDEBUG", "darr.c", "main.c", "-o", "main");
  } else {
    nom_cmd_append_many(&cmd, 6, "gcc", "-ggdb", "darr.c", "main.c", "-o", "main");
  }
  if(!nom_run_sync(cmd)) {
    return 1;
  }
  int idup = argc;
  char** arg_dup = argv;
  if(idup > 1 && strcmp(argv[1], ".") == 0) {
    run("main", arg_dup);
  } else {
    arg_dup++;
    run("main", arg_dup);
  }
  // build_with_lib(files, libs, 1, 1);
  /*
  if(argc >= 3 && strcmp(argv[1], "-build") == 0 && strcmp(argv[2], "run") == 0) {
    for(int i = 0; i < sizeof(files) / sizeof(files[0]); i++) {
      if(strcmp(files[i], program_name) == 0) {
        // char* args[] = {"-l", NULL};
        // run(base(files[i]), args);
      }
      if(!run("./str_tests/build", argv)) {
        nom_log(NOM_PANIC, "failed to run ./str_tests/build");
        return 1;
      }
    }
  }
  */
  return 0;
}
