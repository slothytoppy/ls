#include <dirent.h>
#include <errno.h>
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

void fill_stat(char* x, struct stat* fi) {
  if(lstat(x, fi) < 0) {
    printf("%s", strerror(errno));
  }
}

void print_type(char* x) {
  struct stat fi;
  fill_stat(x, &fi);
  if(S_ISDIR(fi.st_mode)) {
    printf("%s", "d");
  } else if(S_ISLNK(fi.st_mode)) {
    printf("%s", "l");
  }
}

void print_perms(char* x) {
  struct stat fi;
  fill_stat(x, &fi);
  print_type(x);
  printf("%s", (fi.st_mode & S_IRUSR) ? "r" : "-");
  printf("%s", (fi.st_mode & S_IWUSR) ? "w" : "-");
  printf("%s", (fi.st_mode & S_IXUSR) ? "x" : "-");
  printf("%s", (fi.st_mode & S_IRGRP) ? "r" : "-");
  printf("%s", (fi.st_mode & S_IWGRP) ? "w" : "-");
  printf("%s", (fi.st_mode & S_IXGRP) ? "x" : "-");
  printf("%s", (fi.st_mode & S_IROTH) ? "r" : "-");
  printf("%s", (fi.st_mode & S_IWOTH) ? "w" : "-");
  printf("%s ", (fi.st_mode & S_IXOTH) ? "x" : "-");
}

void print_size(char* x) {
  struct stat fi;
  fill_stat(x, &fi);
  printf("\e[0;32m%ld\e[0m ", fi.st_size);
  // printf("%ld ", fi.st_size);
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
  render->path = (char**)realloc(render->path, (render->count + 1) * sizeof(render->path));
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

struct renderer render(char* path) {
  struct dirent* dirent;
  struct stat fi;
  struct renderer rend = {0};
  if(stat(path, &fi) < 0) {
    strerror(errno);
    return rend;
  }
  DIR* dir = opendir(path);
  for(int i = 0; (dirent = readdir(dir)) != NULL; i++) {
    char* dname = dirent->d_name;
    if(strlen(dname) <= 2 && dname[0] == '.' || dname[0] == '.' && dname[1] == '.') {
      continue;
    }
    renderer_append(&rend, dname);
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
