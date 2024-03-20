#include "str_utils.h"
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void check_stat(char* x, struct stat* fi) {
  if(lstat(x, fi) < 0) {
    printf("%s", strerror(errno));
  }
}

void print_type(char* x) {
  struct stat fi;
  check_stat(x, &fi);
  if(S_ISDIR(fi.st_mode)) {
    printf("%s", "d");
  } else if(S_ISLNK(fi.st_mode)) {
    printf("%s", "l");
  }
}

void print_perms(char* x) {
  struct stat fi;
  check_stat(x, &fi);
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
  check_stat(x, &fi);
  printf("%ld ", fi.st_size);
}

int main() {
  char buffer[4096];
  DIR* dir = opendir(getcwd(buffer, sizeof(buffer)));
  if(dir == NULL) {
    return 1;
  }
  int len = str_len("hello");
  struct dirent* dirent;
  while((dirent = readdir(dir)) != NULL) {
    char* dname = dirent->d_name;
    if(str_len(dname) <= 2 && dname[0] == '.' || dname[0] == '.' && dname[1] == '.') {
      continue;
    }
    print_perms(dname);
    print_size(dname);
    printf("%s\n", dirent->d_name);
  }
  closedir(dir);
  return 0;
}
