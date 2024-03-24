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

struct renderer {
  // implemenetd
  char** file;
  char** type;
  char** perms;
  char** size;
  char** path;
  int count;
  int capacity;
  enum state {
    grid,
    long_view,
    // tree,
  } state;
  // unimplemented
  /*
  struct colors colors // in a different file and unimplemented atm
  int* type;        // TODO: implement this later
  char* separator;  // unused
  int spaces_count; // unused
  bool is_color;    // unused
  */
};

enum field {
  File,
  Type,
  Perms,
  Size,
  Path,
} field;

void renderer_init(struct renderer* render) {
#ifdef DEBUG
  printf("%s\n", __FUNCTION__);

#endif
  if(render->count > 0) {
    printf("renderer already initialized\n");
    return;
  }
  render->capacity = 256;
  render->count += 1;
  render->file = (char**)calloc(1, render->capacity * sizeof(render->file));
  render->type = (char**)calloc(1, render->capacity * sizeof(render->type));
  render->perms = (char**)calloc(1, render->capacity * sizeof(render->perms));
  render->size = (char**)calloc(1, render->capacity * sizeof(render->size));
  render->path = (char**)calloc(1, render->capacity * sizeof(render->path));
  printf("renderer_init count: %d\n", render->count);
  return;
}

void renderer_over_cap(struct renderer* render, unsigned max) {
  if(render->count <= render->capacity) {
    return;
  }
#ifdef DEBUG
  printf("%s\n", __FUNCTION__);
#endif
  printf("max: %d\n", max);
  render->capacity *= 2;
  render->capacity = max;
  render->file = (char**)realloc(render->file, max * sizeof(char*));
  if(render->file == NULL) {
    printf("in append: file==NULL: %s\n", strerror(errno));
    return;
  }
  render->type = (char**)realloc(render->type, max * sizeof(char*));
  if(render->type == NULL) {
    printf("in append: type==NULL: %s\n", strerror(errno));
    return;
  }
  render->perms = (char**)realloc(render->perms, max * sizeof(char*));
  if(render->perms == NULL) {
    printf("in append: perms==NULL: %s\n", strerror(errno));
    return;
  }
  render->size = (char**)realloc(render->size, max * sizeof(char*));
  if(render->size == NULL) {
    printf("in append: size==NULL: %s\n", strerror(errno));
    return;
  }
  render->path = (char**)realloc(render->path, max * sizeof(char*));
  if(render->path == NULL) {
    printf("in append: path==NULL: %s\n", strerror(errno));
    return;
  }
  render->capacity *= 2;
}

void renderer_append_file(struct renderer* render, unsigned offset, enum field field, void* item) {
#ifdef DEBUG
  printf("%s\n", __FUNCTION__);
#endif
  if(field != File) {
    printf("error in append: type %d was not equal to File\n", field);
    return;
  }
  if(render->count >= render->capacity) {
    render->capacity *= 2;
    render->file = (char**)realloc(render->file, render->capacity + 1 * sizeof(render->file));
  }
  render->file = (char**)realloc(render->file, sizeof(render->file) * render->count + 1);
  render->file[offset] = item;
  // printf("in append_file render->count: %d\n", render->count);
}

void renderer_append_type(struct renderer* render, unsigned offset, enum field field, void* item) {
#ifdef DEBUG
  printf("%s\n", __FUNCTION__);
#endif
  if(field != Type) {
    printf("error in append: type %d was not equal to Type\n", field);
    return;
  }
  if(render->count >= render->capacity) {
    render->capacity *= 2;
    render->type = (char**)realloc(render->type, render->capacity + 1 * sizeof(render->type));
  }
  render->type = (char**)realloc(render->type, render->count + 1 * sizeof(render->type));
  render->type[offset] = item;
  printf("in append_type render->count: %d\n", render->count);
}

void renderer_append_perms(struct renderer* render, unsigned offset, enum field field, void* item) {
#ifdef DEBUG
  printf("%s\n", __FUNCTION__);
#endif
  if(field != Perms) {
    printf("error in append: type %d was not equal to Perms\n", field);
    return;
  }
  if(render->count >= render->capacity) {
    render->capacity *= 2;
    render->perms = (char**)realloc(render->perms, render->capacity + 1 * sizeof(render->perms));
  }
  render->perms = (char**)realloc(render->perms, render->count + 1 * sizeof(render->perms));
  render->perms[offset] = item;
  printf("in append_perms render->count: %d\n", render->count);
}

void renderer_append_size(struct renderer* render, unsigned offset, enum field field, void* item) {
#ifdef DEBUG
  printf("%s\n", __FUNCTION__);
#endif
  if(field != Size) {
    printf("error in append: type %d was not equal to Size\n", field);
    return;
  }
  if(render->count >= render->capacity) {
    render->capacity *= 2;
    render->size = (char**)realloc(render->size, render->capacity + 1 * sizeof(render->size));
  }
  render->size = (char**)realloc(render->size, render->count + 1 * sizeof(render->size));
  render->size[offset] = item;
  printf("in append_size render->count: %d\n", render->count);
}

void renderer_append_path(struct renderer* render, unsigned offset, enum field field, void* item) {
#ifdef DEBUG
  printf("%s\n", __FUNCTION__);
#endif
  if(field != Path) {
    printf("error in append: type %d was not equal to Path\n", field);
    return;
  }
  if(render->count >= render->capacity) {
    render->capacity *= 2;
    render->path = (char**)realloc(render->path, render->capacity * sizeof(render->path));
  }
  render->path = (char**)realloc(render->path, render->count + 1 * sizeof(render->perms));
  render->path[offset] = item;
}

void renderer_append(struct renderer* render, unsigned offset, enum field field, void* item) {
#ifdef DEBUG
  printf("%s\n", __FUNCTION__);
#endif
  if(render->count == 0) {
    renderer_init(render);
  }
  render->count += 1;
  if(field == File) {
    renderer_append_file(render, offset, File, item);
  } else if(field == Type) {
    renderer_append_type(render, offset, Type, item);
  } else if(field == Perms) {
    renderer_append_perms(render, offset, Perms, item);
  } else if(field == Size) {
    renderer_append_size(render, offset, Size, item);
  } else if(field == Path) {
    renderer_append_path(render, offset, Path, item);
  } else {
    printf("UNKNOWN field in field enum\n");
  }
  // dont increment render->count here, increment it elsewhere
  printf("renderer_append count: %d\n", render->count);
  return;
}

/*
void renderer_append_many(struct renderer* rend, unsigned offset, int count, ...) {
#ifdef DEBUG
  printf("%s\n", __FUNCTION__);
#endif
  va_list args;
  va_start(args, count);
  int i = 0;
  while(i < count) {
    void* item = va_arg(args, void*);
    i++;
  }
  va_end(args);
}
*/

void fill_stat(char* x, struct stat* fi) {
#ifdef DEBUG
  printf("%s\n", __FUNCTION__);
#endif
  // TEMP: making this return on failure without printing anything
  if(lstat(x, fi) < 0) {
    // printf("%s\n", strerror(errno));
    return;
  }
}

char _perms[4] = "rwx-";
char _type[3] = "dl.";

char* render_type(struct renderer* rend, int offset, char* path) {
#ifdef DEBUG
  printf("%s\n", __FUNCTION__);
#endif
  struct stat fi;
  fill_stat(path, &fi);
  char* file = malloc(sizeof(char*));
  if(S_ISDIR(fi.st_mode)) {
    file[0] = _type[0];
  } else if(S_ISLNK(fi.st_mode)) {
    file[0] = _type[1];
  } else {
    file[0] = _type[2];
  }
  renderer_append(rend, offset, Type, file);
  // printf("type: %s\n", rend->type[offset]);
  return file;
}

void render_perms(struct renderer* rend, int offset, char* path) {
  if(!path || offset < 0) {
    return;
  }
#ifdef DEBUG
  printf("%s\n", __FUNCTION__);
#endif
  struct stat fi;
  fill_stat(path, &fi);
  char* file = calloc(1, strlen(path) + 10);
  file[0] = (fi.st_mode & S_IRUSR) ? _perms[0] : _perms[3];
  file[1] = (fi.st_mode & S_IWUSR) ? _perms[1] : _perms[3];
  file[2] = (fi.st_mode & S_IXUSR) ? _perms[2] : _perms[3];
  file[3] = (fi.st_mode & S_IRUSR) ? _perms[0] : _perms[3];
  file[4] = (fi.st_mode & S_IRUSR) ? _perms[1] : _perms[3];
  file[5] = (fi.st_mode & S_IRUSR) ? _perms[2] : _perms[3];
  file[6] = (fi.st_mode & S_IRUSR) ? _perms[0] : _perms[3];
  file[7] = (fi.st_mode & S_IRUSR) ? _perms[1] : _perms[3];
  file[8] = (fi.st_mode & S_IRUSR) ? _perms[2] : _perms[3];
  if(rend->perms == NULL) {
    renderer_append(rend, offset, Perms, file);
  } else {
    rend->perms[offset] = file;
  }
}

void render_size(struct renderer* rend, int offset, char* path) {
#ifdef DEBUG
  printf("%s\n", __FUNCTION__);
#endif
  if(!path || offset < 0) {
    return;
  }
  struct stat fi;
  fill_stat(path, &fi);
  long long fi_size = fi.st_size;
  char* rend_path = rend->size[offset];
  char* suffixes[] = {"", "k", "m", "g"}; // first entry was B, removed it because i didnt
                                          // need to know bytes for something that already
                                          // clearly is in bytes
  int suffixIndex = 0;
  while(fi_size >= 1024 && suffixIndex < 3) {
    fi_size /= 1024;
    suffixIndex++;
  }
  if(rend->size == NULL) {
    renderer_append(rend, offset, Size, suffixes[suffixIndex]);
  } else {
    rend->size[offset] = suffixes[suffixIndex];
  }
  snprintf(rend->size[offset], 10, " %lld%s ", fi_size, suffixes[suffixIndex]);
}

void render_file(struct renderer* rend, int offset, char* path) {
  if(!path) {
    return;
  }
#ifdef DEBUG
  printf("%s\n", __FUNCTION__);
#endif
  int path_len = strlen(path);
  int total_len = path_len;
  char* file = calloc(1, total_len + 2);
  strncat(file, path, path_len);
  if(rend->state == long_view) {
    strcat(file, "\n");
  }
  if(rend->state == grid) {
    strncat(file + strlen(file), " ", 2);
  }
  renderer_append(rend, offset, File, file);
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

void render_path(struct renderer* rend, enum state state, int offset, char* path) {
#ifdef DEBUG
  printf("%s\n", __FUNCTION__);
#endif
  char* file;
  if(state == grid) {
    file = calloc(1, strlen(path));
    if(file == NULL) {
      printf("%s\n", strerror(errno));
    }
    strcat(file, path);
    strcat(file, " ");
    if(offset % 6 == 0) {
      //  strcat(file, "\n");
    }
  } else if(state == long_view) {
    render_type(rend, offset, path);
    render_perms(rend, offset, path);
    render_size(rend, offset, path);
    size_t type_len, perms_len, size_len, file_len;
    type_len = strlen(rend->type[offset]);
    perms_len = strlen(rend->perms[offset]);
    size_len = strlen(rend->size[offset]);
    file_len = strlen(rend->file[offset]);
    file = calloc(1, type_len + perms_len + size_len + file_len);
    strcat(file, rend->type[offset]);
    strcat(file, rend->perms[offset]);
    strcat(file, rend->size[offset]);
    strcat(file, rend->file[offset]);
  }
  render_file(rend, offset, path);
  rend->path[offset] = path;
  // renderer_append(rend, offset, Path, file);
  return;
}

struct renderer render(char* path, enum state state) {
#ifdef DEBUG
  printf("%s\n", __FUNCTION__);
#endif
  struct dirent* dirent;
  struct stat fi;
  struct renderer rend = {0};
  DIR* dir = opendir(path);
  rend.state = state;
  if(stat(path, &fi) < 0 || dir == NULL) {
    printf("%s\n", strerror(errno));
    return rend;
  }
  int i = 0;
  while((dirent = readdir(dir)) != NULL) {
    char* dname = dirent->d_name;
    if(strlen(dname) <= 2 && dname[0] == '.' || dname[0] == '.' && dname[1] == '.') {
      continue;
    }
    render_path(&rend, state, i, dname);
    i++;
  }
  closedir(dir);
  return rend;
}

void print_view(struct renderer render) {
#ifdef DEBUG
  printf("%s\n", __FUNCTION__);
#endif
  for(int i = 0; i < render.count; i++) {
    printf("%s", render.path[i]);
  }
}
// TODO: set up appending colors, 'd', '.' and 'l' at the beginning, permissions, time, and owner
// size in the renderer

int main(int argc, char** argv) {
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
  print_view(rend);
}
