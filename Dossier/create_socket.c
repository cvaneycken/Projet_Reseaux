#include <netinet/in.h> /* * sockaddr_in6 */
#include <sys/types.h> /* sockaddr_in6 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "create_socket.h"
#include <errno.h>      /* errno */
#define _OPEN_SYS_SOCK_IPV6 1

/* Creates a socket and initialize it
 * @source_addr: if !NULL, the source address that should be bound to this socket
 * @src_port: if >0, the port on which the socket is listening
 * @dest_addr: if !NULL, the destination address to which the socket should send data
 * @dst_port: if >0, the destination port to which the socket should be connected
 * @return: a file descriptor number representing the socket,
 *         or -1 in case of error (explanation will be printed on stderr)
 */
int create_socket(struct sockaddr_in6 *source_addr, int src_port, struct sockaddr_in6 *dest_addr, int dst_port){
  //making descriptor
  //17 bc of UDP protocol
  int option = 1;
  int descriptor=socket(AF_INET6,SOCK_DGRAM,0);
  if(descriptor==-1){
    fprintf(stderr, "Fail: socket function\n");
    return -1;
  }
  if(setsockopt(descriptor,SOL_SOCKET,(SO_REUSEPORT | SO_REUSEADDR| SO_KEEPALIVE),(char *)&option,sizeof(option))==-1){
    fprintf(stderr, "Fail: setsockopt function, %s\n", strerror(errno));
    return -1;
  }
  //bounding server
  if(source_addr!=NULL){
    //@src_port: if >0, the port on which the socket is listening
    //cast src_port to "in_port_t" bc of sin6_port's type:
    /* struct sockaddr_in6 {
     * sa_family_t     sin6_family;   //
     * in_port_t       sin6_port;     // port
     * uint32_t        sin6_flowinfo; // IPv6 flow
     * struct in6_addr sin6_addr;     // IPv6
     * uint32_t        sin6_scope_id; // Scope ID (new in 2.4
     *};
     */
    if(src_port>0){
      source_addr->sin6_port=htons(src_port);
    }
    if(source_addr->sin6_family==AF_INET6){
      int err=bind(descriptor,(const struct sockaddr*)source_addr,sizeof(struct sockaddr_in6));
      if(err==-1){
        fprintf(stderr, "Fail:bind function, %s\n",strerror(errno));
        return -1;
      }
    }
  }

  //connecting client
  if(dest_addr!=NULL){
    if(dst_port>0){
      dest_addr->sin6_port=htons(dst_port);
    }
    if(dest_addr->sin6_family==AF_INET6){
      int err2=connect(descriptor,(const struct sockaddr *)dest_addr,sizeof(struct sockaddr_in6));
      if(err2==-1){
        fprintf(stderr, "Fail:connect function\n");
        return -1;
      }
  }
}
  return descriptor;
}
