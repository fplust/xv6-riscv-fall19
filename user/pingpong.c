#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  int parent_fd[2];
  int child_fd[2];
  pipe(parent_fd);
  pipe(child_fd);

  if(fork() == 0) {
    write(child_fd[1], "pong", 5);
    char buf[32];
    read(parent_fd[0], buf, sizeof(buf));
    int pid = getpid();
    printf("%d: received %s\n", pid, buf);
  } else {
    write(parent_fd[1], "ping", 5);
    char buf[32];
    read(child_fd[0], buf, sizeof(buf));
    int pid = getpid();
    printf("%d: received %s\n", pid, buf);
  }

  exit();
}
