#ifndef RENDER_H
#define RENDER_H

#include "./common_headers.h"
#include "./logger.h"

#include "./darr.h"

typedef enum {
  size,
  time,
  dir,
  source,
  reg,
  executable, // size and exe should be diff colors
  lnk,
  reset,
} colors;

typedef struct Colors {
  colors color;
  char* file_colors[];
} Colors;

typedef enum Field {
  File,
  Type,
  Perms,
  Size,
  Path,
} Field;

typedef enum state {
  grid,
  long_view,
  // unimplemented
  tree,
} state;

typedef struct {
  // no perms or type offset because they have constant length
  unsigned perms_offset;
  unsigned type_offset;
  unsigned size_offset;
  unsigned file_offset;
  unsigned path_offset;

  unsigned user_offset;
  unsigned group_offset;
  unsigned owner_offset;
} Offset;

typedef struct {
  int long_view;
  int one_per_line;
  int grid_view;
  int tree_view;
  // file options
  int show_inode;
  int show_help;
  int show_binary;
  int show_bytes;
  int show_group;
  int show_links_count;
  int show_modified;
  int show_flags;
  int show_blocksize;
  int show_created_time;
  int show_accessed_time;
  int show_total_size;
  int no_show_permissions;
  int no_show_file_size;
  int no_show_user;
  int no_show_time;
  int show_header;
  // recursion options
  int list_dirs_only;
  int recursion_depth;
  int recurse;
  // color options
  int colors;
  int colors_supported;
  // input options
  int read_from_stdin;
  // FILTERING AND SORTING
  int reverse_sort; // doesnt do anything but take the way you sort and reverses it
  // DISPLAY OPTIONS
  int show_all;
  int show_almost_all;
  int list_files_only;
  // git options
  int show_git;
  int no_git;
  // link options
  int dereference_links;
  int show_absolute_path;
} Options;

struct renderer {
  state state;
  darr file;
  darr type;
  darr perms;
  darr size;
  darr path;
  Options options;
  Colors Colors;
};

void fill_stat(char* x, struct stat* fi);
char* render_file(struct renderer* rend, char* path);
char* render_type(struct renderer* rend, char* path);
char* render_perms(struct renderer* rend, char* path);
char* render_size(struct renderer* rend, char* path);
int render_path(struct renderer* rend);
struct renderer render(char* path, enum state state);
void debug_print(char* fmt, ...);
void print_path(struct renderer rend);
void println(char*, ...);

// int render_path_test(struct renderer* rend);
// char* render_file_color(struct renderer* rend);
#endif // RENDER_H
