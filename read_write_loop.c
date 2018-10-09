//INSPIRED BY https://www.binarytides.com/multiple-socket-connections-fdset-select-linux/
#include "read_write_loop.h"
#include <netinet/in.h> /* * sockaddr_in6 */
#include <sys/types.h> /* sockaddr_in6 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <unistd.h>

/*A[ fread(stdin) -> write(socket) ]  --  B[ read(socket) -> fwrite(stdout) ]
 *B[ fread(stdin) -> write(socket) ]  --  A[ read(socket) -> fwrite(stdout) ]
 * fonctions conseillées: select, poll
 */
/* Loop reading a socket and printing to stdout,
 * while reading stdin and writing to the socket
 * @sfd: The socket file descriptor. It is both bound and connected.
 * @return: as soon as stdin signals EOF
 */
void read_write_loop(int sfd){
  int wfd,rfd,wfd2,rfd2,activity;
  char buffer[1024];
  fd_set writeSet;
  fd_set readSet;

  while(1){

    memset(buffer,'\0',1024);
    //Reading from stdin
    rfd2=read(STDIN_FILENO,buffer,1024);
    if(rfd2!=1024){
      fprintf(stderr, "Err:Couldn't read from stdin\n");
      exit(-1);
    }
    FD_SET(rfd2,(fd_set *)&readSet);
    //Writing on socket
    wfd=write(sfd,buffer,strlen(buffer));
    if(wfd<0){
      fprintf(stderr, "Err at writing on socket\n");
      exit(-1);
    }
    FD_SET(wfd,(fd_set *)&writeSet);
    memset(buffer,'\0',1024);
    //Reads from socket
    rfd=read(sfd,buffer,1024);
    if(rfd!=1024){
      fprintf(stderr, "Err at reading from socket\n");
      exit(-1);
    }
    FD_SET(rfd,(fd_set *)&readSet);
    //Write to stdout
    wfd2=write(STDOUT_FILENO,buffer,strlen(buffer));
    if(wfd2<0){
      fprintf(stderr, "Err=Failed at Writing on stdout\n");
      exit(-1);
    }
    FD_SET(wfd2,(fd_set *)&writeSet);

    activity=select(sfd,(fd_set*)&readSet,(fd_set *)&writeSet,NULL,NULL);
    if(activity==-1){
      fprintf(stderr, "Err: Select\n");
    }

  }

}
