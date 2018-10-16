
#include "wait_for_client.h"
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>


/* Block the caller until a message is received on sfd,
 * and connect the socket to the source addresse of the received message
 * @sfd: a file descriptor to a bound socket but not yet connected
 * @return: 0 in case of success, -1 otherwise
 * @POST: This call is idempotent, it does not 'consume' the data of the message,
 * and could be repeated several times blocking only at the first call.
 */
 /*
  *Fonctions conseillées: recvfrom, connect
  */
  // INSPIRED FROM mrahrauld's github
int wait_for_client(int sfd){
  char *buf=malloc(sizeof(char)*1024);
  struct sockaddr_in6 *src_addr=malloc(sizeof(struct sockaddr_in6));
  if(src_addr==NULL){
    fprintf(stderr, "Err: malloc failed\n");
    return -1;
  }
  socklen_t addr_len=sizeof(struct sockaddr_in6);
  int err=recvfrom(sfd,(void *)buf,sizeof(char)*1024,MSG_PEEK,(struct sockaddr *)src_addr,&addr_len);
  if(err==-1){
    fprintf(stderr, "Err: recvfrom\n");
    return -1;
  }
err=connect(sfd,(struct sockaddr *)src_addr,addr_len);
  if(err==-1){
    fprintf(stderr, "Err: connect\n");
    return -1;
  }
  return 0;
}
