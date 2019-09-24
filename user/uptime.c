#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  int ut = uptime();
  if(ut < 0){
    fprintf(2, "uptime: failed\n");
  } else {
    fprintf(2, "%d\n", ut);
  }

  exit();
}
