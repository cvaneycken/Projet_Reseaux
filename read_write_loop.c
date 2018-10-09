//INSPIRED BY https://www.binarytides.com/multiple-socket-connections-fdset-select-linux/

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
  int wfd,rfd;
  int wfd2,rfd2;
  char buffer[256];
  fd_set writeSet;
  fd_set readSet;

  while(1){

    bzero(buffer,256);
    fgets(buffer,255,stdin);
    //Reading from stdin
    rfd2=read(stdin,buffer,255);
    if(rfd2!=255){
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
    bzero(buffer,256);
    //Reads from socket
    rfd=read(sfd,buffer,255);
    if(rfd<0){
      fprintf(stderr, "Err at reading from socket\n");
      exit(-1);
    }
    FD_SET(rfd,(fd_set *)&readSet);
    //Write to stdout
    wfd2=write(stdout,buffer,strlen(buffer));
    if(wfd2<0){
      fprintf(stderr, "Err=Failed at Writing on stdout\n");
      exit(-1);
    }
    FD_SET(wfd2,(fd_set *)&writeSet);

    int activity=select(sfd,(fd_set*)&readSet,(fd_set *)&writeSet,NULL,NULL);

  }

}
