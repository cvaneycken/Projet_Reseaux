#include "packet_interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <sys/ioctl.h>

int main(int argc, char const *argv[]) {
  char buf[1024];
  while(1){
    scanf("%1023s", buf);
    if(buf!=NULL){
      printf("Il y a qqchose: %s\n", buf);
    }
  }
}
