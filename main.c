#include "render.h"

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
    println("cwd: %s %s: %s", getcwd(cwd, 4096), x, strerror(errno));
    return;
  }
  debug_log("leaving %s", __FUNCTION__);
}

char* render_file(struct renderer* rend, char* path) {
  debug_log("entering %s", __FUNCTION__);
  if(!path) {
    return NULL;
  }
  append(&rend->file, path);
  debug_log("leaving %s", __FUNCTION__);
  return path;
}

char* render_type(struct renderer* rend, char* path) {
  debug_log("entering %s", __FUNCTION__);
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
  append(&rend->type, file);
  debug_log("leaving %s", __FUNCTION__);
  return file;
}

char* render_perms(struct renderer* rend, char* path) {
  debug_log("entering %s", __FUNCTION__);
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
  append(&rend->perms, perms);
  debug_log("perms=%s", perms);
  debug_log("leaving %s", __FUNCTION__);
  return perms;
}

char* render_size(struct renderer* rend, char* path) {
  debug_log("entering %s", __FUNCTION__);
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
  append(&rend->size, file);
  debug_log("leaving %s", __FUNCTION__);
  return file;
}

int render_path(struct renderer* rend) {
  debug_log("entering %s", __func__);
  int file_len, size_len, type_len, perms_len, total_len;
  char *i_type, *i_perms, *i_size, *i_file;
  for(int i = 0; i < rend->file.count; i++) {
    if(rend->state == long_view) {
      type_len = rend->type.count;
      perms_len = rend->perms.count;
      size_len = rend->size.count;
      file_len = rend->file.count;
      i_type = rend->type.items[i];
      i_perms = rend->perms.items[i];
      i_size = rend->size.items[i];
      i_file = rend->file.items[i];
      total_len = file_len + size_len + type_len + perms_len;
      char* x = calloc(1, total_len);
      snprintf(x, total_len, "%s%s %s %s", i_type, i_perms, i_size, i_file);
      append(&rend->path, x);
    } else {
      append(&rend->path, rend->file.items[i]);
    }
  }
  debug_log("leaving %s", __func__);
  return 1;
}

void print_path(struct renderer rend) {
  debug_log("entering %s", __FUNCTION__);
  struct winsize winsize;
  ioctl(0, TIOCGWINSZ, &winsize);
  unsigned count = 0;
  int idup;
  for(int i = 0; i < rend.path.count; i++) {
    printf("%s\n", (char*)rend.path.items[i]);
  }
  debug_log("leaving %s", __FUNCTION__);
}

void aligned_grid(struct renderer* rend) {
  debug_log("entering %s", __FUNCTION__);
  if(rend->state == long_view) {
    unsigned max_len, max_size = 0;
    /* // was used for finding the longest string in rend->file.items[i]
    max_len = strlen(rend->file.items[0]);
    char* file;
    for(int i = 0; i < rend->file.count; i++) {
      unsigned len = strlen(rend->file.items[i]);
      max_len = len > max_len ? len : max_len;
    }
    */
    for(int i = 0; i < rend->size.count; i++) {
      unsigned len = strlen(rend->size.items[i]);
      char* size = calloc(1, max_size);
      if(len <= max_size) {
        memset(size, ' ', max_size - len);
        strcat(size, rend->size.items[i]);
        rend->size.items[i] = size;
        debug_log("%s\n", size);
      }
      max_size = len > max_size ? len : max_size;
    }
    debug_log("max_size=%d\n", max_size);
    debug_log("max_len=%d", max_len);
  }
  debug_log("leaving %s", __FUNCTION__);
  return;
}

struct renderer render(char* path, enum state state) {
  debug_log("entering %s", __FUNCTION__);
  struct dirent* dirent;
  struct stat fi;
  struct renderer rend = {0};
  DIR* dir = opendir(path);
  rend.state = state;
  if(stat(path, &fi) < 0 || dir == NULL) {
    println("%s: %s", path, strerror(errno));
    return rend;
  }
  unsigned i = 0;
  while((dirent = readdir(dir)) != NULL) {
    char* dname = dirent->d_name;
    if(strlen(dname) <= 2 && dname[0] == '.' || dname[0] == '.' && dname[1] == '.') {
      continue;
    }
    if(state == long_view) {
      render_size(&rend, dname);
      render_type(&rend, dname);
      aligned_grid(&rend);
      render_perms(&rend, dname);
    }
    render_file(&rend, dname);
    i++;
  }
  render_path(&rend); // render_path assumes all three other fields are filled out
  closedir(dir);
  debug_log("leaving %s", __FUNCTION__);
  return rend;
}

void println(char* fmt, ...) {
  debug_log("entering %s", __FUNCTION__);
  va_list args;
  va_start(args, fmt);
  vfprintf(stdout, fmt, args);
  printf("\n");
  va_end(args);
  debug_log("leaving %s", __FUNCTION__);
}
