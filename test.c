#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./render.h"

int mass_append(unsigned x);
int test_render_file(struct renderer* rend, char* path);
int test_render_type(struct renderer* rend, char* path);
int test_render_perms(struct renderer* rend, char* path);
int test_render_size(struct renderer* rend, char* path);
int test_render_path(struct renderer* rend, char* path);
int print_renderer(struct renderer rend, unsigned i);
int render_path(struct renderer* rend);
int render_path_test(struct renderer* rend);

/*
int main(void) {
  printf("hello world\n");
  struct renderer rend = {0};
if(!mass_append(1000)) {
  return 1;
}
struct dirent* dirent;
DIR* dir = opendir(".");
while((dirent = readdir(dir)) != NULL) {
  char* file = dirent->d_name;
  if(strlen(file) <= 2 && file[0] == '.' || file[0] == '.' && file[1] == '.') {
    continue;
  }
  test_render_file(&rend, file);
  test_render_type(&rend, file);
  test_render_perms(&rend, file);
  test_render_size(&rend, file);
}
render_path(&rend);
//  print_renderer(rend, 100);
return 0;
}
*/

int mass_append(unsigned x) {
  struct renderer rend = {0};
  int i = 0;
  for(i = 0; i < x; i++) {
    char* file = calloc(1, 32);
    char* type = calloc(1, 32);
    char* perms = calloc(1, 32);
    char* size = calloc(1, 32);
    char* path = calloc(1, 32);
    strcat(file, "file: ");
    strcat(type, "type: ");
    strcat(perms, "perms: ");
    strcat(size, "size: ");
    strcat(path, "path: ");
    snprintf(file, 32, "%d", i);
    snprintf(type, 32, "%d", i);
    snprintf(perms, 32, "%d", i);
    snprintf(size, 32, "%d", i);
    snprintf(path, 32, "%d", i);
    renderer_append_file(&rend, file);
    renderer_append_type(&rend, type);
    renderer_append_perms(&rend, perms);
    renderer_append_size(&rend, size);
    renderer_append_path(&rend, path);
    if(!rend.file[i] || !rend.type[i] || !rend.perms[i] || !rend.size[i] || !rend.path[i]) {
      return 0;
    }
  }
  print_renderer(rend, i);
  printf("i: %d\n", i);
  return 1;
}

int print_renderer(struct renderer rend, unsigned y) {
  printf("entering %s\n", __func__);
  for(int i = 0; i < y; i++) {
    if(i < rend.file_count) {
      if(rend.file[i] != NULL) {
        printf("file: %s\n", rend.file[i]);
      }
    }
    if(i < rend.type_count) {
      if(rend.type[i] != NULL) {
        printf("type: %s\n", rend.type[i]);
      }
    }
    if(i < rend.perms_count) {
      if(rend.perms[i] != NULL) {
        printf("perms: %s\n", rend.perms[i]);
      }
    }
    if(i < rend.size_count) {
      if(rend.size[i] != NULL) {
        printf("size: %s\n", rend.size[i]);
      }
    }
    if(i < rend.path_count) {
      if(rend.path[i] != NULL) {
        printf("path: %s\n", rend.path[i]);
      }
    }
  }
  printf("leaving %s\n", __func__);
  return 1;
}

int test_render_file(struct renderer* rend, char* path) {
  if(!render_file(rend, path)) {
    return 0;
  }
  // no render_file in render.c
  return 0;
}

int test_render_type(struct renderer* rend, char* path) {
  if(!render_type(rend, path)) {
    return 0;
  }
  return 1;
}

int test_render_perms(struct renderer* rend, char* path) {
  if(!render_perms(rend, path)) {
    return 0;
  }
  return 1;
}

int test_render_size(struct renderer* rend, char* path) {
  if(!render_size(rend, path)) {
    return 0;
  }
  return 1;
}

int test_render_path(struct renderer* rend, char* path) {
  if(!render_path(rend)) {
    return 0;
  }
  return 1;
}

int render_path_test(struct renderer* rend) {
  render_path(rend);
  unsigned f_count = rend->file_count;
  unsigned t_count = rend->type_count;
  unsigned perms_count = rend->perms_count;
  unsigned path_count = rend->path_count;
  if(rend->state == long_view) {
    if(f_count != t_count || f_count != perms_count || f_count != path_count) {
      return 0;
    }
  }
  for(int i = 0; i < f_count; i++) {
    if(rend->path[i] == NULL) {
      printf("rend->path[%d] is null, hole in darr\n", i);
      return 0;
    }
    printf("path[%d]=%s\n", i, rend->path[i]);
  }
  return 1;
}
