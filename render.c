#include <dirent.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <termio.h>
#include <unistd.h>

char* file_colors[] = {
    "\e[0;32m",
    "\033[38:5:4m",
    "\e[0;34m",
    "\033[38:5:208m",
    "\033[38:5:255m",
    "\e[0;32m",
    "\033[38:5:73m",
    "\033[0m",
};

typedef enum colors {
  green,
  blue,
  orange,
  light_green,
  light_blue,
  white,
} colors;

typedef enum renderable_colors {
  size = green,
  time = blue,
  dir = blue,
  source = orange,
  reg = white,
  executable = green, // size and exe should be diff colors
  lnk = light_blue,
  reset_colors = white,
} renderable_colors;

struct color {
  char* fg;
  char* bg;
};

enum {
  grid,
  line,
  tree,
};

struct renderer {
  struct color color;
  char** path;
  char* separator;
  int count;
  int capacity;
  int spaces_count;
  bool is_color;
};

void renderer_append(struct renderer* render, char* path) {
  if(render->count == 0) {
    render->capacity = 256;
    render->path = (char**)calloc(1, sizeof(render->path));
    render->path[0] = path;
    render->count += 1;
    return;
  }

  render->count += 1;
  // render->path = (char**)realloc(render->path, (render->count + 1) * sizeof(render->path));
  render->path = (char**)realloc(render->path, (render->count + 1) * sizeof(char*));
  render->path[render->count - 1] = path;
  render->path[render->count] = NULL;

  if(render->count >= render->capacity) {
    render->capacity *= 2;
    render->path = (char**)realloc(render->path, render->capacity * sizeof(char*));
    if(render->path == NULL) {
      printf("%s", strerror(errno));
      return;
    }
  }
  return;
}

void renderer_append_many(struct renderer* rend, int count, ...) {
  va_list args;
  va_start(args, count);
  int i = 0;
  while(i < count) {
    char* item = va_arg(args, char*);
    renderer_append(rend, item);
    i++;
  }
  va_end(args);
}

void fill_stat(char* x, struct stat* fi) {
  if(lstat(x, fi) < 0) {
    printf("%s", strerror(errno));
  }
}

char perms[4] = "rwx-";
char type[3] = "dl.";

char* render_type(char* path) {
  struct stat fi;
  fill_stat(path, &fi);
  char* file = malloc(sizeof(char*));
  if(S_ISDIR(fi.st_mode)) {
    file[0] = type[0];
  } else if(S_ISLNK(fi.st_mode)) {
    file[0] = type[1];
  } else {
    file[0] = type[2];
  }
  return file;
}

void render_perms(struct renderer* rend, int offset, char* path) {
  if(!path || offset < 0) {
    return;
  }
  struct stat fi;
  renderer_append(rend, path);
  fill_stat(rend->path[offset], &fi);
  char* file = calloc(1, strlen(path) + 10);
  strncat(file, render_type(path), 1);
  file[1] = (fi.st_mode & S_IRUSR) ? perms[0] : perms[3];
  file[2] = (fi.st_mode & S_IWUSR) ? perms[1] : perms[3];
  file[3] = (fi.st_mode & S_IXUSR) ? perms[2] : perms[3];
  file[4] = (fi.st_mode & S_IRUSR) ? perms[0] : perms[3];
  file[5] = (fi.st_mode & S_IRUSR) ? perms[1] : perms[3];
  file[6] = (fi.st_mode & S_IRUSR) ? perms[2] : perms[3];
  file[7] = (fi.st_mode & S_IRUSR) ? perms[0] : perms[3];
  file[8] = (fi.st_mode & S_IRUSR) ? perms[1] : perms[3];
  file[9] = (fi.st_mode & S_IRUSR) ? perms[2] : perms[3];
  file[10] = ' ';
  strncat(file + 10, path, strlen(path));
  // printf("file:%s\n", file);
  rend->path[offset] = file;
}

void print_size(struct renderer* rend, int offset, char* path) {
  if(!path || offset < 0) {
    return;
  }
  struct stat fi;
  fill_stat(rend->path[offset], &fi);
}

void print_color(char* x, enum renderable_colors colors) {
  printf("%s%s%s\n", file_colors[colors], x, file_colors[white]);
}

void print_file_color(char* x) {
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

struct renderer render(char* path) {
  struct dirent* dirent;
  struct stat fi;
  struct renderer rend = {0};
  if(stat(path, &fi) < 0) {
    strerror(errno);
    return rend;
  }
  DIR* dir = opendir(path);
  int i = 0;
  while((dirent = readdir(dir)) != NULL) {
    char* dname = dirent->d_name;
    if(strlen(dname) <= 2 && dname[0] == '.' || dname[0] == '.' && dname[1] == '.') {
      continue;
    }
    render_perms(&rend, i, dname);
    i++;
  }
  closedir(dir);
  return rend;
}

void print_renderer(struct renderer render) {
  for(int i = 0; i < render.count; i++) {
    printf("%s\n", render.path[i]);
  }
}

// TODO: set up appending colors, 'd', '.' and 'l' at the beginning, permissions, time, and owner
// size in the renderer

int main(void) {
  struct renderer rend = render(".");
  print_renderer(rend);
}
