#include <stdlib.h> /* EXIT_X */
#include <stdio.h> /* fprintf */
#include <unistd.h> /* getopt */

#include "real_address.h"
#include "create_socket.h"
#include "read_write_loop.h"
#include "wait_for_client.h"


int main( int argc, char *argv[] )  {
  int opt;
  int i=0;
	while ((opt = getopt(argc, argv, "f")) != -1) {
		switch (opt) {
      case 'f':
        printf("yo chose f with %s\n", optarg);
        break;
      }
    }
  for(i=0;i<argc;i++){
    if(strcmp(argv[i],"::1")==0){
      printf("The argument supplied is %s\n", argv[i]);
    }
  }
   return 0;
}
