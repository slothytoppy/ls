#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
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
