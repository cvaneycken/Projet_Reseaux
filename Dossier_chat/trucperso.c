#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <signal.h>
#include <poll.h>
#include <errno.h>

#include "read_write_loop.h"

void read_write_loop(int sfd){
  char buffer[1024];
  //stdinState==1 if it was read
  int stdinState=0;
  //socketState==1 if it contains buffer to write to stdout
  int socketState=0;

  while(1){
    if(stdinState==0){
      memset(buffer,'\0',sizeof(char)*1024);
      scanf("%1023s", buffer);
      if(buffer!=NULL) stdinState=1;//Ready to write on socket
    }
    if(stdinState==1){
      if(write(sfd,buffer,err1)<0){
        fprintf(stderr, "Err at writing on socket %s\n",strerror(errno));
        exit(-1);
      }
      socketState=1;//Socket ready to write on stdout
      stdinState=0;
    }
    if(socketState==1){
      memset(buffer,'\0',sizeof(char)*1024);
      //read from socket
      if(read(sfd,buffer,sizeof(char)*1024)<0){
        fprintf(stderr, "Err at reading from socket\n");
        exit(-1);
      }
      socketState=0; //Socket ready to get written on
      printf("%s \n",buffer);
    }
  }
}
int main(int argc, char const *argv[]) {
  read_write_loop(0);
  return 0;
}
