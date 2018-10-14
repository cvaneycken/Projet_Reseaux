#include "packet_interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

int main(int argc, char const *argv[]) {
  uint8_t a: 2;
  a=1;
  printf("the size (in bits) of %d is %lu\n",a,(int)CHAR_BIT*sizeof(a));
  printf("the size (in bits) of PTYPE_DATA is %lu\n",(int)CHAR_BIT*sizeof(PTYPE_DATA));
  return 0;
}
