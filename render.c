#include <dirent.h>
#include <errno.h>
// #include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <termio.h>
#include <unistd.h>

#include "./render.h"

void renderer_append_file(struct renderer* rend, char* item) {
  if(rend->file_count == 0) {
    rend->file_count += 1;
    rend->file = (char**)calloc(1, (rend->file_count + 1) * sizeof(char**));
    rend->file[0] = item;
    rend->file[1] = NULL;
  } else {
    rend->file_count += 1;
    rend->file = realloc(rend->file, (rend->file_count + 1) * sizeof(char**));
    rend->file[rend->file_count - 1] = item;
    rend->file[rend->file_count] = NULL;
  }
}

void renderer_append_type(struct renderer* rend, char* item) {
  if(rend->type_count == 0) {
    rend->type_count += 1;
    rend->type = calloc(1, (rend->type_count + 1) * sizeof(char**));
    rend->type[0] = item;
    rend->type[1] = NULL;
  } else {
    rend->type_count += 1;
    rend->type = realloc(rend->type, (rend->type_count + 1) * sizeof(char**));
    rend->type[rend->type_count - 1] = item;
    rend->type[rend->type_count] = NULL;
  }
}

void renderer_append_perms(struct renderer* rend, char* item) {
  if(rend->perms_count == 0) {
    rend->perms_count += 1;
    rend->perms = calloc(1, (rend->perms_count + 1) * sizeof(char**));
    rend->perms[0] = item;
    rend->perms[1] = NULL;
  } else {
    rend->perms_count += 1;
    rend->perms = realloc(rend->perms, (rend->perms_count + 1) * sizeof(char**));
    rend->perms[rend->perms_count - 1] = item;
    rend->perms[rend->perms_count] = NULL;
  }
}

void renderer_append_size(struct renderer* rend, char* item) {
  if(rend->size_count == 0) {
    rend->size_count += 1;
    rend->size = calloc(1, (rend->size_count + 1) * sizeof(char**));
    rend->size[0] = item;
    rend->size[1] = NULL;
  } else {
    rend->size_count += 1;
    rend->size = realloc(rend->size, (rend->size_count + 1) * sizeof(char**));
    rend->size[rend->size_count - 1] = item;
    rend->size[rend->size_count] = NULL;
  }
}

void renderer_append_path(struct renderer* rend, char* item) {
  if(rend->path_count == 0) {
    rend->path_count += 1;
    rend->path = calloc(1, (rend->path_count + 1) * sizeof(char**));
    rend->path[0] = item;
    rend->path[1] = NULL;
  } else {
    rend->path_count += 1;
    rend->path = realloc(rend->path, (rend->path_count + 1) * sizeof(char**));
    rend->path[rend->path_count - 1] = item;
    rend->path[rend->path_count] = NULL;
  }
}

void fill_stat(char* x, struct stat* fi) {
#ifdef DEBUG
  printf("entering %s\n", __FUNCTION__);
#endif
  if(!x) {
    return;
  }
  // TEMP: making this return on failure without printing anything
  if(lstat(x, fi) < 0) {
    printf("%s\n", strerror(errno));
    return;
  }
#ifdef DEBUG
  printf("leaving %s\n", __FUNCTION__);
#endif
}

char* render_file(struct renderer* rend, char* path) {
#ifdef DEBUG
  printf("entering %s\n", __FUNCTION__);
#endif
  if(!path) {
    return NULL;
  }
  renderer_append_file(rend, path);
#ifdef DEBUG
  printf("leaving %s\n", __FUNCTION__);
#endif
  return path;
}

char* render_type(struct renderer* rend, char* path) {
#ifdef DEBUG
  printf("entering %s\n", __FUNCTION__);
#endif
  if(!path) {
    return NULL;
  }
  char _type[3] = "dl.";
  struct stat fi;
  fill_stat(path, &fi);
  char* file = calloc(1, 2);
  if(S_ISDIR(fi.st_mode)) {
    file[0] = _type[0];
  } else if(S_ISLNK(fi.st_mode)) {
    file[0] = _type[1];
  } else {
    file[0] = _type[2];
  }
  renderer_append_type(rend, file);
#ifdef DEBUG
  printf("leaving %s\n", __FUNCTION__);
#endif
  return file;
}

char* render_perms(struct renderer* rend, char* path) {
#ifdef DEBUG
  printf("entering %s\n", __FUNCTION__);
#endif
  if(!path) {
    return NULL;
  }
  char _perms[4] = "rwx-";
  struct stat fi;
  fill_stat(path, &fi);
  char* perms = calloc(1, strlen(path) + 10);
  perms[0] = (fi.st_mode & S_IRUSR) ? _perms[0] : _perms[3];
  perms[1] = (fi.st_mode & S_IWUSR) ? _perms[1] : _perms[3];
  perms[2] = (fi.st_mode & S_IXUSR) ? _perms[2] : _perms[3];
  perms[3] = (fi.st_mode & S_IRUSR) ? _perms[0] : _perms[3];
  perms[4] = (fi.st_mode & S_IRUSR) ? _perms[1] : _perms[3];
  perms[5] = (fi.st_mode & S_IRUSR) ? _perms[2] : _perms[3];
  perms[6] = (fi.st_mode & S_IRUSR) ? _perms[0] : _perms[3];
  perms[7] = (fi.st_mode & S_IRUSR) ? _perms[1] : _perms[3];
  perms[8] = (fi.st_mode & S_IRUSR) ? _perms[2] : _perms[3];
  renderer_append_perms(rend, perms);
#ifdef DEBUG
  printf("perms=%s\n", perms);
  printf("leaving %s\n", __FUNCTION__);
#endif
  return perms;
}

char* render_size(struct renderer* rend, char* path) {
#ifdef DEBUG
  printf("entering %s\n", __FUNCTION__);
#endif
  if(!path) {
    return NULL;
  }
  struct stat fi;
  fill_stat(path, &fi);
  long long fi_size = fi.st_size;
  char* file = calloc(1, 10 + 1);
  char* suffixes[] = {"", "k", "m", "g"}; // first entry was B, removed it because i didnt
                                          // need to know bytes for something that already
                                          // clearly is in bytes
  unsigned suffixIndex = 0;
  while(fi_size >= 1024 && suffixIndex < 3) {
    fi_size /= 1024;
    suffixIndex++;
  }
  snprintf(file, sizeof(fi_size) + 1, "%lld%s", fi_size, suffixes[suffixIndex]);
  renderer_append_size(rend, file);
#ifdef DEBUG
  printf("leaving %s\n", __FUNCTION__);
#endif
  return file;
}

/*
void print_file_color(char* x) {
#ifdef DEBUG
  printf("%s\n", __FUNCTION__);
#endif
  struct stat fi;
  int visited = 0;
  fill_stat(x, &fi);
  if(S_ISDIR(fi.st_mode)) {
    if(!visited)
      print_color(x, dir);
    visited = 1;
  } else if(S_ISLNK(fi.st_mode)) {
    if(!visited)
      print_color(x, lnk);
    visited = 1;
  }
  if(S_IXOTH & fi.st_mode || S_IXUSR & fi.st_mode || S_IXGRP & fi.st_mode) {
    if(!visited)
      print_color(x, executable);
    visited = 1;
  }
  char* hello = strdup(x);
  hello = strrchr(hello, '.');
  if(hello != NULL) {
    if(strcmp(hello, ".c") == 0) {
      if(!visited)
        print_color(x, source);
      visited = 1;
    }
  } else {
    if(!visited)
      print_color(x, reg);
    visited = 1;
  }
}
*/

int render_path(struct renderer* rend) {
#ifdef DEBUG
  printf("entering %s\n", __func__);
#endif
  /* if(rend->file_count == 0 || rend->type_count == 0 || rend->perms_count == 0 || rend->size_count) {
    return 0;
  } */
  if(rend->path_count != 0) {
    rend->path_count = 0;
    return 0;
  }
  for(int i = 0; i < rend->file_count; i++) {
    unsigned extra_bytes = 3;
    char* perms;
    if(rend->state == long_view) {
      perms = calloc(1, strlen(rend->type[i]) + strlen(rend->perms[i]) + strlen(rend->size[i]) + strlen(rend->file[i]) + extra_bytes + 1);
      strcat(perms, rend->type[i]);
      strcat(perms, rend->perms[i]);
      strcat(perms, " ");
      strcat(perms, rend->size[i]);
      strcat(perms, " ");
    } else {
      perms = calloc(1, strlen(rend->file[i]));
    }
    strcat(perms, rend->file[i]);
    renderer_append_path(rend, perms);
  }
  for(int i = 0; i < rend->path_count; i++) {
    printf("%s\n", rend->path[i]);
  }
#ifdef DEBUG
  printf("leaving %s\n", __func__);
#endif
  return 1;
}

struct renderer render(char* path, enum state state) {
#ifdef DEBUG
  printf("entering %s\n", __FUNCTION__);
#endif
  struct dirent* dirent;
  struct stat fi;
  struct renderer rend = {0};
  DIR* dir = opendir(path);
  rend.state = state;
  char* temp;
  if(stat(path, &fi) < 0 || dir == NULL) {
    printf("%s: %s\n", path, strerror(errno));
    return rend;
  }
  unsigned i = 0;
  while((dirent = readdir(dir)) != NULL) {
    char* dname = dirent->d_name;
    if(strlen(dname) <= 2 && dname[0] == '.' || dname[0] == '.' && dname[1] == '.') {
      continue;
    }
    render_file(&rend, dname);
    render_size(&rend, dname);
    render_type(&rend, dname);
    render_perms(&rend, dname);
    i++;
  }
  render_path(&rend);
  closedir(dir);
#ifdef DEBUG
  printf("leaving %s\n", __FUNCTION__);
#endif
  return rend;
}

// TODO: set up appending colors, 'd', '.' and 'l' at the beginning, permissions, time, and owner
// size in the renderer

int main(int argc, char* argv[]) {
  enum state state;
  char* path = calloc(1, 4096);
  if(argc > 1) {
    state = long_view;
    path = argv[1];
  } else {
    state = grid;
    path = getcwd(path, 4096);
  }
  struct renderer rend = render(path, state);
}
