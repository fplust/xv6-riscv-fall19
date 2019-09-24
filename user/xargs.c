#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

int
getcmd(char *buf, int nbuf)
{
  memset(buf, 0, nbuf);
  gets(buf, nbuf);
  if(buf[0] == 0) // EOF
    return -1;
  return 0;
}

int
main(int argc, char *argv[])
{
  if(argc < 2){
    fprintf(2, "Usage: xargs command [argv]");
    exit();
  }
  static char buf[100];
  while(getcmd(buf, sizeof(buf)) >= 0){
    // printf("debug: %s", buf);
    if(fork() == 0) {
      char *cargv[MAXARG];
      memset(cargv, 0, sizeof(*cargv));
      for(int i=0; i<argc-1; i++) {
        cargv[i] = argv[i+1];
      }
      int len = argc - 1;
      for(int j=0; j<sizeof(buf); j++) {
        if(j == 0 && buf[j] == ' ') {
          continue;
        } else if (j == 0 && buf[j] != ' ') {
          cargv[len] = &buf[j];
          len += 1;
          continue;
        } else if (buf[j-1] != ' ' && buf[j] == ' ') {
          buf[j] = '\0';
          continue;
        } else if ((buf[j-1] == ' ' || buf[j-1] == '\0') && buf[j] != ' ') {
          cargv[len] = &buf[j];
          len += 1;
          continue;
        } else if (buf[j] == '\n') {
          // printf("debug: end\n");
          buf[j] = '\0';
          break;
        } else {
          continue;
        }
      }
      exec(cargv[0], cargv);
      exit();
    }
    wait();
  }
  exit();
}
