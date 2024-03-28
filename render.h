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
    // unimplemented
    // tree,
  } state;
  // unimplemented
  /*
  struct colors colors // in a different file and unimplemented atm
      int* type;       // TODO: implement this later
  char* separator;     // unused
  int spaces_count;    // unused
  bool is_color;       // unused
  */
};

enum field {
  File,
  Type,
  Perms,
  Size,
  Path,
};

void renderer_append_file(struct renderer* rend, char* item);
void renderer_append_type(struct renderer* rend, char* item);
void renderer_append_perms(struct renderer* rend, char* item);
void renderer_append_size(struct renderer* rend, char* item);
void renderer_append_path(struct renderer* rend, char* item);
char* render_file(struct renderer* rend, char* path);
char* render_type(struct renderer* rend, char* path);
char* render_perms(struct renderer* rend, char* path);
char* render_size(struct renderer* rend, char* path);
int render_path(struct renderer* rend);
int render_path_test(struct renderer* rend);
