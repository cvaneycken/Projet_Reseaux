//SOURCE: https://github.com/sCreami/cnp3-p1/blob/master/inginious/ta2/read_write_loop.c

#include "read_write_loop.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <strings.h>
#include <errno.h>

/* Loop reading a socket and printing to stdout,
 * while reading stdin and writing to the socket
 * @sfd: The socket file descriptor. It is both bound and connected.
 * @return: as soon as stdin signals EOF
 */
void read_write_loop(int sfd)
{
    ssize_t nBytes;
    fd_set fds;
    char buffer[1024];
    struct timeval timeVal =(struct timeval){
      .tv_sec = 5,
      .tv_usec =0,
    };
    while(1){
      FD_ZERO(&fds);
      FD_SET(sfd,&fds);
      FD_SET(STDIN_FILENO, &fds);

      if(select(FD_SETSIZE, &fds, NULL, NULL, &timeVal) == -1){
        fprintf(stderr, "Err: select function, %s\n",strerror(errno));
        exit(-1);
      }
      if(FD_ISSET(sfd,&fds)){
        nBytes=read(sfd,buffer,sizeof(buffer));
        if(nBytes==0) return;
        if(nBytes<0){
          fprintf(stderr, "Err: reading from socket, %s\n",strerror(errno));
          exit(-1);
        }
        if(write(STDOUT_FILENO,buffer,nBytes)==-1){
          fprintf(stderr, "Err: writing on stdout %s\n",strerror(errno));
          exit(-1);
        }
        memset((void *)buffer,0,sizeof(char)*1024);
      }
     if(FD_ISSET(STDIN_FILENO,&fds)){
       nBytes=read(STDIN_FILENO,buffer,sizeof(buffer));
       if(nBytes==0) return;
       if(nBytes<0){
         fprintf(stderr, "Err: reading from stdin %s\n",strerror(errno));
         exit(-1);
       }
       if(write(sfd,buffer,nBytes)<0){
         fprintf(stderr, "Err: writing on socket %s\n",strerror(errno));
         exit(-1);
       }
       memset((void *)buffer,0,sizeof(char)*1024);
     }
    }

}
