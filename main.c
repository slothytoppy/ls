#include "render.h"

#include <assert.h>
#include <stdbool.h>

int main(int argc, char** argv) {
  char* path = calloc(1, 4096);
  enum state state;
  if(argc > 1) {
    state = long_view;
    path = argv[1];
  } else {
    state = grid;
    path = getcwd(path, 4096);
  }
  struct renderer rend = render(path, state);
  // render_path_colors(&rend);
  print_path(rend);
  return 0;
}

void fill_stat(char* x, struct stat* fi) {
  debug_log("entering %s", __FUNCTION__);
  if(!x) {
    return;
  }
  char cwd[4096];
  // TEMP: making this return on failure without printing anything
  if(lstat(x, fi) < 0) {
    printf("cwd: %s %s: %s\n", getcwd(cwd, 4096), x, strerror(errno));
    return;
  }
  debug_log("leaving %s", __FUNCTION__);
}

void render_file(struct renderer* rend, char* path) {
  debug_log("entering %s", __FUNCTION__);
  if(!path) {
    return;
  }
  append(&rend->file, path);
  debug_log("leaving %s", __FUNCTION__);
  return;
}

void render_type(struct renderer* rend) {
  debug_log("entering %s", __FUNCTION__);
  if(rend->file.count == 0) {
    return;
  }
  char _type[3] = "dl.";
  struct stat fi;
  for(int i = 0; i < rend->file.count; i++) {
    fill_stat(rend->file.items[i], &fi);
    char* file = calloc(1, 3);
    if(S_ISDIR(fi.st_mode)) {
      file[0] = _type[0];
    } else if(S_ISLNK(fi.st_mode)) {
      file[0] = _type[1];
    } else {
      file[0] = _type[2];
    }
    append(&rend->type, file);
  }
  debug_log("leaving %s", __FUNCTION__);
}

void render_perms(struct renderer* rend) {
  debug_log("entering %s", __FUNCTION__);
  if(rend->file.count == 0) {
    return;
  }
  char _perms[4] = "rwx-";
  for(int i = 0; i < rend->file.count; i++) {
    struct stat fi;
    fill_stat(rend->file.items[i], &fi);
    char* perms = calloc(1, sizeof(fi.st_size) + 1);
    perms[0] = (fi.st_mode & S_IRUSR) ? _perms[0] : _perms[3];
    perms[1] = (fi.st_mode & S_IWUSR) ? _perms[1] : _perms[3];
    perms[2] = (fi.st_mode & S_IXUSR) ? _perms[2] : _perms[3];
    perms[3] = (fi.st_mode & S_IRGRP) ? _perms[0] : _perms[3];
    perms[4] = (fi.st_mode & S_IWGRP) ? _perms[1] : _perms[3];
    perms[5] = (fi.st_mode & S_IXGRP) ? _perms[2] : _perms[3];
    perms[6] = (fi.st_mode & S_IROTH) ? _perms[0] : _perms[3];
    perms[7] = (fi.st_mode & S_IWOTH) ? _perms[1] : _perms[3];
    perms[8] = (fi.st_mode & S_IXOTH) ? _perms[2] : _perms[3];
    debug_log("perms=%s", perms);
    append(&rend->perms, perms);
  }
  debug_log("leaving %s", __FUNCTION__);
  return;
}

void render_size(struct renderer* rend) {
  debug_log("entering %s", __FUNCTION__);
  if(rend->file.count == 0) {
    return;
  }
  struct stat fi;
  const char* const suffixes[] = {"", "k", "m", "g"};
  for(int i = 0; i < rend->file.count; i++) {
    unsigned suffixIndex = 0;
    fill_stat(rend->file.items[i], &fi);
    long long fi_size = fi.st_size;
    while(fi_size >= 1024 && suffixIndex < 3) {
      fi_size /= 1024;
      suffixIndex += 1;
    }
    char* size = calloc(1, sizeof(fi_size));
    snprintf(size, sizeof(fi_size) + 1, "%lld%s", fi_size, suffixes[suffixIndex]);
    append(&rend->size, size);
  }
  debug_log("leaving %s", __FUNCTION__);
  return;
}

void print_path(struct renderer rend) {
  debug_log("entering %s", __FUNCTION__);
  struct winsize winsize;
  ioctl(0, TIOCGWINSZ, &winsize);
  unsigned count = 0;
  int idup;
  assert(rend.path.count > 0);
  for(int i = 0; i < rend.path.count; i++) {
    printf("%s\n", (char*)rend.path.items[i]);
  }
  debug_log("leaving %s", __FUNCTION__);
}

unsigned longest_string(darr* darr) {
  unsigned max_len = 0, max_size = 0;
  for(int i = 0; i < darr->count; i++) {
    unsigned len = strlen(darr->items[i]);
    max_len = len > max_len ? len : max_len;
  }
  return max_len;
}

struct renderer render(char* path, enum state state) {
  debug_log("entering %s", __FUNCTION__);
  struct dirent* dirent;
  struct stat fi;
  struct renderer rend = {0};
  rend.options.colors_supported = true;
  DIR* dir = opendir(path);
  rend.state = state;
  if(stat(path, &fi) < 0 || dir == NULL) {
    debug_log("%s: %s", path, strerror(errno));
    return rend;
  }
  unsigned i = 0;
  while((dirent = readdir(dir)) != NULL) {
    char* dname = dirent->d_name;
    if(strlen(dname) <= 2 && dname[0] == '.' || dname[0] == '.' && dname[1] == '.') {
      continue;
    }
    render_file(&rend, dname);
    i++;
  }

  render_type(&rend);
  render_perms(&rend);
  render_size(&rend);
  render_path_colors(&rend);
  // render_path(&rend); // non colored version of render_path_colors
  closedir(dir);
  debug_log("leaving %s", __FUNCTION__);
  return rend;
}

int render_path(struct renderer* rend) {
  debug_log("entering %s", __FUNCTION__);
  if(rend->file.count == 0) {
    return 0;
  }
  char* file;
  unsigned file_len = 0, size_len = 0, perms_len = 0, type_len = 0, total_len = 0;
  file_len = longest_string(&rend->file);
  size_len = longest_string(&rend->size);
  perms_len = longest_string(&rend->perms);
  type_len = longest_string(&rend->type);
  total_len = file_len + size_len + perms_len + type_len;
  for(int i = 0; i < rend->file.count; i++) {
    file = calloc(1, total_len);
    if(rend->state == long_view) {
      strcat(file, rend->type.items[i]);
      strcat(file, rend->perms.items[i]);
      strcat(file, " ");
      strcat(file, rend->size.items[i]);
      strcat(file, " ");
    }
    strcat(file, rend->file.items[i]);
    append(&rend->path, file);
  }
  debug_log("leaving %s", __FUNCTION__);
  return 1;
}

int render_path_colors(struct renderer* rend) {
  debug_log("entering %s", __FUNCTION__);
  char *file, *perms, *link;
  unsigned file_len = 0, type_len = 0, size_len = 0, perms_len = 0, link_len = 0, total_len = 0;
  // debug_log("dir=\e[0;34mdir\e[0m");
  if(rend->options.colors_supported == false) {
    return 0;
  }
  darr colors_darr = {0};
  for(int i = 0; i < sizeof(file_colors) / sizeof(file_colors[0]); i++) {
    append(&colors_darr, file_colors[i]);
  }
  unsigned columns = 2; // a column is something like files or size
  unsigned colors_len = longest_string(&colors_darr) * columns + strlen(file_colors[reset]) * columns;
  free(colors_darr.items);
  file_len = longest_string(&rend->file);
  type_len = longest_string(&rend->type);
  size_len = longest_string(&rend->size);
  perms_len = longest_string(&rend->perms);
  total_len = file_len + type_len + size_len + perms_len + colors_len;
  printf("total=%d\n", total_len);
  for(int i = 0; i < rend->file.count; i++) {
    file = calloc(1, total_len);
    strcat(file, rend->type.items[i]);
    strcat(file, rend->perms.items[i]);
    strcat(file, " ");
    unsigned cur_szlen = strlen(rend->size.items[i]);
    if(cur_szlen < 3) {
      for(int i = 0; i + cur_szlen < 3; i++) {
        strcat(file, " ");
      }
    }
    strcat(file, file_colors[size]);
    strcat(file, rend->size.items[i]);
    strcat(file, file_colors[reset]);
    strcat(file, " ");
    file = strcat(file, file_colors[source]);
    file = strcat(file, rend->file.items[i]);
    file = strcat(file, file_colors[reset]);
    append(&rend->path, file);
  }
  debug_log("leaving %s", __FUNCTION__);
  return 1;
}
