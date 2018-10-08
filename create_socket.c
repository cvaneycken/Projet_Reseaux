#include <netinet/in.h> /* * sockaddr_in6 */
#include <sys/types.h> /* sockaddr_in6 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>


/* Creates a socket and initialize it
 * @source_addr: if !NULL, the source address that should be bound to this socket
 * @src_port: if >0, the port on which the socket is listening
 * @dest_addr: if !NULL, the destination address to which the socket should send data
 * @dst_port: if >0, the destination port to which the socket should be connected
 * @return: a file descriptor number representing the socket,
 *         or -1 in case of error (explanation will be printed on stderr)
 */
int create_socket(struct sockaddr_in6 *source_addr, int src_port, struct sockaddr_in6 *dest_addr, int dst_port){
//Testing arguments
  if(source_addr==NULL||src_port<0||dest_addr==NULL||dst_port<0){
    fprintf(stderr, "Fail: Problem with given arguments\n");
    return -1;
  }
  //making descriptor
  int descriptor=socket(AF_INET6,SOCK_STREAM,0);
  if(descriptor==-1){
    fprintf(stderr, "Fail: socket function\n");
    return -1;
  }
  //bounding server
  int err=bind(descriptor,(const struct sockaddr *)source_addr,sizeof(source_addr));
  if(err==-1){
    fprintf(stderr, "Fail:bind function\n");
    return -1;
  }
  //connecting client
  err=connect(descriptor,(const struct sockaddr *)dest_addr,sizeof(dest_addr));
  if(err==-1){
    fprintf(stderr, "Fail:connect function\n");
    return -1;
  }
  return descriptor;
}
