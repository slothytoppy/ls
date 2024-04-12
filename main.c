#include "./render.h"

#include <assert.h>
#include <stdbool.h>

#define print_fn_name printf("%s\n", __FUNCTION__)

int main(int argc, char** argv) {
  print_fn_name;
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

int fill_stat(char* path, struct renderer* render) {
  print_fn_name;
  if(path == NULL) {
    return 0;
  }
  if(stat(path, &render->fi) < 0) {
    printf("%s\n", strerror(errno));
    return 0;
  }
  return 1;
}

int render_file(struct renderer* rend, char* path) {
  print_fn_name;
  darr_append(&rend->file, path);
  return 1;
}

int render_type(struct renderer* rend) {
  print_fn_name;
  if(rend->file.count <= 0) {
    return 0;
  }
  for(int i = 0; i < rend->file.count; i++) {
    fill_stat(rend->file.items[i], rend);
    if(S_ISDIR(rend->fi.st_mode)) {
      darr_append(&rend->type, "d");
    } else if(S_ISLNK(rend->fi.st_mode)) {
      darr_append(&rend->type, "l");
    } else if(S_ISREG(rend->fi.st_mode)) {
      darr_append(&rend->type, ".");
    } else {
      return 0;
    }
  }
  return 1;
}

int render_perms(struct renderer* rend) {
  print_fn_name;
  if(rend->file.count <= 0) {
    return 0;
  }
  char _perms[4] = "rwx-";
  for(int i = 0; i < rend->file.count; i++) {
    fill_stat(rend->file.items[i], rend);
    char* perms = calloc(1, sizeof(rend->fi.st_size) + 1);
    perms[0] = (rend->fi.st_mode & S_IRUSR) ? _perms[0] : _perms[3];
    perms[1] = (rend->fi.st_mode & S_IWUSR) ? _perms[1] : _perms[3];
    perms[2] = (rend->fi.st_mode & S_IXUSR) ? _perms[2] : _perms[3];
    perms[3] = (rend->fi.st_mode & S_IRGRP) ? _perms[0] : _perms[3];
    perms[4] = (rend->fi.st_mode & S_IWGRP) ? _perms[1] : _perms[3];
    perms[5] = (rend->fi.st_mode & S_IXGRP) ? _perms[2] : _perms[3];
    perms[6] = (rend->fi.st_mode & S_IROTH) ? _perms[0] : _perms[3];
    perms[7] = (rend->fi.st_mode & S_IWOTH) ? _perms[1] : _perms[3];
    perms[8] = (rend->fi.st_mode & S_IXOTH) ? _perms[2] : _perms[3];
    darr_append(&rend->perms, perms);
  }
  return 0;
}

int render_size(struct renderer* rend) {
  print_fn_name;
  if(rend->file.count <= 0) {
    return 0;
  }
  const char* const suffixes[] = {"", "k", "m", "g"};
  for(int i = 0; i < rend->file.count; i++) {
    fill_stat(rend->file.items[i], rend);
    unsigned suffixIndex = 0;
    long long fi_size = rend->fi.st_size;
    while(fi_size >= 1024 && suffixIndex < sizeof(suffixes) / sizeof(suffixes[0])) {
      fi_size /= 1024;
      suffixIndex += 1;
    }
    char* num = calloc(1, sizeof(fi_size) + 1);
    snprintf(num, sizeof(fi_size) + 1, "%lld", fi_size);
    char* size = calloc(1, strlen(num) + sizeof(fi_size));
    for(int i = 0; suffixIndex == 0 && strlen(size) + strlen(num) < 3; i++) {
      strcat(size, " ");
    }
    for(int i = 0; suffixIndex > 0 && strlen(size) + strlen(num) + suffixIndex < 3; i++) {
      strcat(size, " ");
    }
    strcat(size, num);
    strcat(size, suffixes[suffixIndex]);
    strcat(size, " ");
    darr_append(&rend->size, size);
  }
  return 1;
}

unsigned longest_string(darr* darr) {
  print_fn_name;
  unsigned max_len = 0, max_size = 0;
  for(int i = 0; i < darr->count; i++) {
    unsigned len = strlen(darr->items[i]);
    max_len = len > max_len ? len : max_len;
  }
  return max_len;
}

int render_path(struct renderer* rend) {
  print_fn_name;
  unsigned file_len = 0, size_len = 0, type_len = 0, perms_len = 0;
  type_len = longest_string(&rend->type);
  perms_len = longest_string(&rend->perms);
  size_len = longest_string(&rend->size);
  file_len = longest_string(&rend->file);
  unsigned space_count = 2;
  unsigned total_len = type_len + perms_len + size_len + file_len + space_count;
  for(int i = 0; i < rend->file.count; i++) {
    char* path = calloc(1, total_len);
    if(rend->state == long_view) {
      strcat(path, rend->type.items[i]);
      strcat(path, rend->perms.items[i]);
      strcat(path, " ");
      strcat(path, rend->size.items[i]);
    }
    strcat(path, rend->file.items[i]);
    darr_append(&rend->path, path);
  }
  return 1;
}

struct renderer render(char* path, enum state state) {
  print_fn_name;
  struct renderer renderer = {0};
  renderer.state = state;
  if(path == NULL || state > tree) {
    return renderer;
  }
  struct dirent* dirent;
  DIR* dir = opendir(path);
  if(dir == NULL) {
    printf("%s\n", strerror(errno));
    return renderer;
  }
  for(int i = 0; (dirent = readdir(dir)) != NULL; i++) {
    if(strlen(dirent->d_name) <= 2 && dirent->d_name[0] == '.' || dirent->d_name[1] == '.') {
      continue;
    }
    render_file(&renderer, dirent->d_name);
  }
  render_size(&renderer);
  render_type(&renderer);
  render_perms(&renderer);
  render_path(&renderer);
  return renderer;
}

void print_path(struct renderer rend) {
  print_fn_name;
  for(int i = 0; i < rend.path.count; i++) {
    printf("%s\n", rend.path.items[i]);
  }
  return;
}
