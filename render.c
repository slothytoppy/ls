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

struct renderer {
  // implemenetd
  char** file;
  char** type;
  char** perms;
  char** size;
  char** path;
  unsigned file_count;
  unsigned type_count;
  unsigned perms_count;
  unsigned size_count;
  unsigned path_count;
  unsigned capacity;
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

void renderer_append_file(struct renderer* render, enum field field, void* item) {
#ifdef DEBUG
  printf("entering %s\n", __FUNCTION__);
#endif
  const unsigned f_count = render->file_count;
  if(field != File) {
    printf("error in append: type %d was not equal to File\n", field);
    return;
  }
  if(f_count <= 0) {
    render->capacity = 256;
    render->file = (char**)calloc(1, sizeof(char*));
    render->file[0] = item;
    render->file_count += 1;
    return;
  }
  if(f_count >= render->capacity) {
    render->capacity *= 2;
    render->file = (char**)realloc(render->file, render->capacity + 1 * sizeof(render->file));
    render->file[render->file_count] = item;
    render->file_count += 1;
    return;
  }
  render->file = (char**)realloc(render->file, sizeof(render->file) * render->file_count + 1);
  render->file[render->file_count] = item;
  const char* file_offset = render->file[render->file_count];
  printf("in append_file render->file_count: %d render->file[render->file_count]: %s\n", f_count, file_offset);
#ifdef DEBUG
  printf("leaving %s\n", __FUNCTION__);
#endif
}

void renderer_append_type(struct renderer* render, enum field field, void* item) {
#ifdef DEBUG
  printf("entering %s\n", __FUNCTION__);
#endif
  if(field != Type) {
    printf("error in append: type %d was not equal to Type\n", field);
    return;
  }
  const unsigned rcount = render->type_count;
  if(rcount <= 0) {
    render->capacity = 256;
    render->file = (char**)calloc(1, sizeof(char*));
    render->file[0] = item;
    render->type_count += 1;
    return;
  }
  if(rcount >= render->capacity) {
    render->capacity *= 2;
    render->type = (char**)realloc(render->type, render->capacity + 1 * sizeof(render->type));
    render->type[render->type_count] = item;
    render->type_count += 1;
    return;
  }
  // printf("render->type_count: %d\n", render->type_count);
  // printf("render->type alloc_count: %d\n", render->capacity + 1 * sizeof(render->type));
  render->type = (char**)realloc(render->type, rcount + 1 * sizeof(render->type));
  render->type[rcount] = item;
  char* type_offset = render->type[render->type_count];
  printf("in append_type render->type_count: %d render->type[offset]: %s\n", rcount, type_offset);
#ifdef DEBUG
  printf("leaving %s\n", __FUNCTION__);
#endif
}

void renderer_append_perms(struct renderer* render, enum field field, void* item) {
#ifdef DEBUG
  printf("entering %s\n", __FUNCTION__);
#endif
  if(field != Perms) {
    printf("error in append: type %d was not equal to Perms\n", field);
    return;
  }
  const unsigned rcount = render->perms_count;
  if(rcount <= 0) {
    render->capacity = 256;
    render->perms = calloc(1, sizeof(char*));
    render->perms[render->perms_count] = item;
    render->perms_count += 1;
    return;
  }
  if(rcount >= render->capacity) {
    render->capacity *= 2;
    render->perms = (char**)realloc(render->perms, render->capacity + 1 * sizeof(render->perms));
    render->perms[render->perms_count] = item;
    return;
  }
  render->perms = (char**)realloc(render->perms, rcount + 1 * sizeof(render->perms));
  render->perms[rcount] = item;
  const char* const perms_offset = render->perms[render->perms_count];
  printf("in append_perms render->perms_count: %d render->perms[offset]: %s\n", rcount, perms_offset);
#ifdef DEBUG
  printf("leaving %s\n", __FUNCTION__);
#endif
}

void renderer_append_size(struct renderer* render, enum field field, void* item) {
#ifdef DEBUG
  printf("entering %s\n", __FUNCTION__);
#endif
  if(field != Size) {
    printf("error in append: type %d was not equal to Size\n", field);
    return;
  }
  const unsigned rcount = render->size_count;

  if(rcount <= 0) {
    render->capacity = 256;
    render->size = calloc(1, sizeof(char*));
    render->size[render->size_count] = item;
    render->size_count += 1;
    return;
  }
  if(rcount >= render->capacity) {
    render->capacity *= 2;
    render->size = (char**)realloc(render->size, render->capacity + 1 * sizeof(render->perms));
    render->size[render->size_count] = item;
    return;
  }

  render->size = (char**)realloc(render->size, rcount + 1 * sizeof(render->size));
  render->size[rcount] = item;
  const char* const size_offset = render->size[render->size_count];
  printf("in append_size render->size_count: %d render->size[offset]: %s\n", rcount, size_offset);
#ifdef DEBUG
  printf("leaving %s\n", __FUNCTION__);
#endif
}

void renderer_append_path(struct renderer* render, enum field field, void* item) {
#ifdef DEBUG
  printf("entering %s\n", __FUNCTION__);
#endif
  if(field != Path) {
    printf("error in append: type %d was not equal to Path\n", field);
    return;
  }
  unsigned rcount = render->path_count;
  if(rcount == 0 || render->capacity == 0) {
    render->capacity = 256;
    render->path = calloc(1, sizeof(char*));
    render->path[0] = item;
    render->path_count += 1;
    return;
  }
  if(render->path_count >= render->capacity) {
    render->capacity *= 2;
    render->path = (char**)realloc(render->path, render->capacity * sizeof(render->path));
  }
  render->path = (char**)realloc(render->path, rcount + 1 * sizeof(render->perms));
  render->path[render->path_count] = item;
  char* path_offset = render->path[render->path_count];
  printf("in append_path render->path_count: %d render->path[offset]: %s\n", rcount, path_offset);
#ifdef DEBUG
  printf("leaving %s\n", __FUNCTION__);
#endif
}

void renderer_append(struct renderer* render, enum field field, void* item) {
#ifdef DEBUG
  printf("entering %s\n", __FUNCTION__);
#endif
  unsigned t_count = render->type_count;
  unsigned p_count = render->perms_count;
  unsigned f_count = render->file_count;
  unsigned path_count = render->path_count;
  /*
  printf("%d ", t_count);
  printf("%d ", p_count);
  printf("%d ", f_count);
  printf("%d\n", path_count);
  */
  if(field == File) {
    renderer_append_file(render, File, item);
    printf("RENDER->file=%s\n", render->file[render->file_count - 1]);
    render->file_count += 1;
  } else if(field == Type) {
    renderer_append_type(render, Type, item);
    printf("RENDER->type=%s\n", render->type[render->type_count - 1]);
    render->type_count += 1;
  } else if(field == Perms) {
    renderer_append_perms(render, Perms, item);
    printf("RENDER->perms=%s\n", render->perms[render->perms_count - 1]);
    render->perms_count += 1;
  } else if(field == Size) {
    renderer_append_size(render, Size, item);
    printf("RENDER->size=%s\n", render->size[render->size_count - 1]);
    render->size_count += 1;
  } else if(field == Path) {
#ifdef DEBUG
    printf("appending to path\n");
#endif
    renderer_append_path(render, Path, item);
    printf("RENDER->path=%s\n", render->path[render->path_count - 1]);
    render->path_count += 1;
  } else {
    printf("UNKNOWN field in field enum\n");
    return;
  }
  // dont increment render->count here, increment it elsewhere
#ifdef DEBUG
  printf("leaving %s\n", __FUNCTION__);
#endif
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
  printf("entering %s\n", __FUNCTION__);
#endif
  // TEMP: making this return on failure without printing anything
  if(lstat(x, fi) < 0) {
    printf("%s\n", strerror(errno));
    return;
  }
#ifdef DEBUG
  printf("leaving %s\n", __FUNCTION__);
#endif
}

char _perms[4] = "rwx-";
char _type[3] = "dl.";

char* render_type(struct renderer* rend, char* path) {
#ifdef DEBUG
  printf("entering %s\n", __FUNCTION__);
#endif
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
  renderer_append(rend, Type, file);
#ifdef DEBUG
  printf("leaving %s\n", __FUNCTION__);
#endif
  return file;
}

void render_perms(struct renderer* rend, char* path) {
#ifdef DEBUG
  printf("entering %s\n", __FUNCTION__);
#endif
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
#ifdef DEBUG
  printf("perms=%s\n", perms);
  printf("leaving %s\n", __FUNCTION__);
#endif
  renderer_append(rend, Perms, perms);
  // rend->perms[offset] = perms;
}

void render_size(struct renderer* rend, char* path) {
#ifdef DEBUG
  printf("entering %s\n", __FUNCTION__);
#endif
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
#ifdef DEBUG
  printf("leaving %s\n", __FUNCTION__);
#endif
  renderer_append(rend, Size, file);
}

void render_file(struct renderer* rend, char* path) {
#ifdef DEBUG
  printf("entering %s\n", __FUNCTION__);
#endif
  if(!path) {
    return;
  }
  unsigned total_len = strlen(path);
  char* file = calloc(1, total_len + 2);
  strncat(file, path, total_len);
  if(rend->state == long_view) {
    strcat(file, "\n");
  }
  if(rend->state == grid) {
    strncat(file + strlen(file), " ", 2);
  }
#ifdef DEBUG
  printf("leaving %s\n", __FUNCTION__);
#endif
  renderer_append(rend, File, file);
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

void render_to_path(struct renderer* rend, enum state state, char* path) {
#ifdef DEBUG
  printf("entering %s\n", __FUNCTION__);
#endif
  unsigned buff = 5;
  char* file = calloc(1, sizeof(char*));
  if(file == NULL) {
    printf("%s\n", strerror(errno));
  }
  if(state == grid) {
    strcat(file, path);
    strcat(file, " ");
  } else if(state == long_view) {
    render_type(rend, path);
    render_perms(rend, path);
    render_size(rend, path);
    // render_file(rend, path);
    size_t type_count, perms_count, size_count, file_len, total_len;
    type_count = sizeof(rend->type[rend->type_count - 1]);
    perms_count = sizeof(rend->perms[rend->perms_count - 1]);
    size_count = sizeof(rend->size[rend->size_count - 1]);
    size_t type_len, perms_len, size_len;
    type_len = strlen(rend->type[rend->type_count - 1]);
    perms_len = strlen(rend->perms[rend->perms_count - 1]);
    size_len = strlen(rend->size[rend->size_count - 1]);
    // file_len = sizeof(rend->file[rend->file_count]);
    file = realloc(file, type_len + perms_len + size_len);
    if(file == NULL) {
      printf("%s\n", strerror(errno));
    }
    strcat(file, rend->type[rend->type_count - 1]);
    strcat(file, rend->perms[rend->perms_count - 1]);
    strcat(file, rend->size[rend->size_count - 1]);
  }
  renderer_append(rend, File, file);
#ifdef DEBUG
  printf("rend->file[rend->file_count]: %s\n", rend->file[rend->file_count - 1]);
#endif
  renderer_append(rend, Path, file);
  return;
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
    render_to_path(&rend, state, dname);
    i++;
  }
  closedir(dir);
#ifdef DEBUG
  printf("leaving %s\n", __FUNCTION__);
#endif
  return rend;
}

void print_view(struct renderer render) {
#ifdef DEBUG
  printf("entering %s\n", __FUNCTION__);
#endif
  for(unsigned i = 0; i < render.path_count && render.path[i]; i++) {
    printf("%s", render.path[i]);
  }
#ifdef DEBUG
  printf("leaving %s\n", __FUNCTION__);
#endif
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
  // print_view(rend);
}
