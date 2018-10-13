/*SOURCES:
 *http://www.khmere.com/freebsd_book/src/06/poll_socket.c.html
 *https://github.com/mrahrauld/CN_Gr37/blob/master/read_write_loop.c
 */
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
  ssize_t err1;
  int err;
  char buffer[1024];
  struct pollfd fds[3];
  fds[0].fd = sfd;
  fds[0].events = POLLIN;
  fds[1].fd = STDIN_FILENO;
  fds[1].events = POLLIN;
  fds[2].fd = STDOUT_FILENO;
  fds[2].events = POLLOUT;

  while(1){
    //printf("w");
    err = poll(fds,2,-1);
    if(err == POLL_ERR){
      perror("Error on poll");
      exit(-1);
    }
    if(fds[1].revents == POLLIN){
      memset(buffer,'\0',1024);
      //read from stdin
      err1=read(STDIN_FILENO,buffer,1023);
      if(err<0){
        fprintf(stderr, "Err:Couldn't read from stdin\n");
        exit(-1);
      }
      //Write on socket
      if(write(sfd,buffer,err1)<0){
        fprintf(stderr, "Err at writing on socket %s\n",strerror(errno));
        exit(-1);
      }
    }
    else if(fds[0].revents == POLLIN){
      memset(buffer,'\0',1024);
      //read from socket
      if(read(sfd,buffer,1024)<0){
        fprintf(stderr, "Err at reading from socket\n");
        exit(-1);
      }
      //Write to STDOUT_FILENO
      if(write(STDOUT_FILENO,buffer,strlen(buffer))<0){
        fprintf(stderr, "Err=Failed at Writing on stdout\n");
        exit(-1);
      }
    }
  }

}
