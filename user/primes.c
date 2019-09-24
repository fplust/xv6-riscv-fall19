#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int process(int left) {
  int p[1];
  int r = read(left, p, sizeof(p));
  if (r == 0) {
    return 0;
  }
  printf("prime %d\n", *p);
  int fd[2];
  pipe(fd);
  if(fork() == 0) {
    close(fd[1]);
    process(fd[0]);
    close(fd[0]);
    exit();
  } else {
    close(fd[0]);
    while (1) {
      int n[1];
      int r = read(left, n, sizeof(n));
      if(r == 0) {
        close(left);
        // fprintf(2, "exit\n");
        return 0;
      } else {
        // fprintf(2, "debug: %d, %d\n", *n, *p);
        if(*n % *p != 0) {
          write(fd[1], n, sizeof(n));
        }
      }
    }
  }
}

int
main(void)
{
  int fd[2];
  pipe(fd);
  if(fork() == 0) {
    close(fd[1]);
    process(fd[0]);
    close(fd[0]);
    exit();
  } else {
    close(fd[0]);
    for(int i=2; i<=35; i++) {
      write(fd[1], &i, sizeof(i));
    }
    close(fd[1]);
  }
  exit();
}
