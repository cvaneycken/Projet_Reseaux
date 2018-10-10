#include <netinet/in.h> /* * sockaddr_in6 */
#include <sys/types.h> /* sockaddr_in6 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "create_socket.h"

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
  int descriptor=socket(AF_INET6,SOCK_DGRAM,0);
  if(descriptor==-1){
    fprintf(stderr, "Fail: socket function\n");
    return -1;
  }
  //bounding server
  if(source_addr!=NULL){
    //@src_port: if >0, the port on which the socket is listening
    //cast src_port to "u_int16m_t" bc of sin6_port's type:
    /* struct sockaddr_in6 {
     *    u_char           sin6_len;      //length of this structure
     *    u_char           sin6_family;   // AF_INET6
     *    u_int16m_t       sin6_port;     // Transport layer port #
     *    u_int32m_t       sin6_flowinfo; // IPv6 flow information
     *    struct in6_addr  sin6_addr;     // IPv6 address
     * };
     */
    if(src_port>0){
      source_addr->sin6_port=(u_int16m_t)src_port;
    }
    int err=bind(descriptor,(const struct sockaddr *)source_addr,sizeof(source_addr));
    fprintf(stderr, "sizeof(source_addr):%d sizeof(struct sockaddr_in6):%d\n",(int)sizeof(source_addr),(int)sizeof(struct sockaddr_in6));
    if(err==-1){
      fprintf(stderr, "Fail:bind function\n");
      return -1;
    }
  }
  //connecting client
  err=connect(descriptor,(const struct sockaddr *)dest_addr,sizeof(dest_addr));
  if(err==-1){
    fprintf(stderr, "Fail:connect function\n");
    return -1;
  }
  return descriptor;
}
