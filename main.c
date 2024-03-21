#include "str_utils.h"
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>

void abort_handler(int signo) {
  printf("%s", strsignal(signo));
  psignal(signo, "\ncaught");
  raise(SIGKILL);
}

void signal_register(int signo) {
  printf("%d\n", getpid());
  struct sigaction sa;
  sa.sa_handler = abort_handler;
  sigemptyset(&sa.sa_mask);
  sigaddset(&sa.sa_mask, SIGKILL);
  if(sigaction(SIGKILL, &sa, NULL) != 0) {
    printf("sigaction failed because of: %d", errno);
  }
  while(1) {
  }
}

// TODO: add colors to print_perms and print_size

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
  size,
  time,
  dir, // TODO: time and dir should be diff colors
  source,
  reg,
  executable, // size and exe should be diff colors
  lnk,
  reset,
} colors;

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

void print_colors(void) {
  colors colors;
  for(int i = 0; i < reset; i++) {
    printf("%s%s%s\n", file_colors[i], "hb", file_colors[reset]);
  }
}

void print_file_color(char* x) {
  struct stat fi;
  int visited = 0;
  fill_stat(x, &fi);
  colors colors;
  if(S_ISDIR(fi.st_mode)) {
    if(!visited)
      printf("%s%s%s ", file_colors[dir], x, file_colors[reset]);
    visited = 1;
  } else if(S_ISLNK(fi.st_mode)) {
    if(!visited)
      printf("%s%s%s ", file_colors[lnk], x, file_colors[reset]);
    visited = 1;
  }
  if(S_IXOTH & fi.st_mode || S_IXUSR & fi.st_mode || S_IXGRP & fi.st_mode) {
    if(!visited)
      printf("%s%s%s ", file_colors[executable], x, file_colors[reset]);
    visited = 1;
  }
  char* hello = strdup(x);
  hello = strrchr(hello, '.');
  if(hello != NULL) {
    if(strcmp(hello, ".c") == 0) {
      if(!visited)
        printf("%s%s%s ", file_colors[source], x, file_colors[reset]);
      visited = 1;
    }
  } else {
    if(!visited)
      printf("%s%s%s ", file_colors[reg], x, file_colors[reset]);
    visited = 1;
  }
}

typedef struct {
  char* file;
  int type;
} file_info;

int main(int argc, char** argv) {
  char buffer[4096];
  int len = str_len("hello");
  int count = 0;
  DIR* dir = opendir(getcwd(buffer, sizeof(buffer)));
  struct dirent* dirent;
  if(dir == NULL) {
    return 1;
  }
  while((dirent = readdir(dir)) != NULL) {
    char* dname = dirent->d_name;
    count += strlen(dirent->d_name);
    file_info file_info = {0};
    if(str_len(dname) <= 2 && dname[0] == '.' || dname[0] == '.' && dname[1] == '.') {
      continue;
    }
    if(argc > 1) {
      if(str_cmp(argv[1], "-l")) {
        print_perms(dname);
        print_size(dname);
        print_file_color(dname);
        printf("\n");
      }
    }
    // TODO: add a grid system:tm: for prettier printing, alignment, and customizability
    if(argc <= 1) {
      struct winsize win;
      ioctl(STDIN_FILENO, TIOCGWINSZ, &win);
      if(count > win.ws_col || count + strlen(dname) > win.ws_col) {
        count = 0;
        printf("newline ");
        // printf("\n");
      }
      print_file_color(dname);
    }
  }
  if(argc <= 1) {
    printf("\n");
  }
  closedir(dir);
  return 0;
}
