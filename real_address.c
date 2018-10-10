// INSPIRED FROM mrahrauld's github
#include <netinet/in.h> /* * sockaddr_in6 */
#include <sys/types.h> /* sockaddr_in6 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "real_address.h"

/* Resolve the resource name to an usable IPv6 address
 * @address: The name to resolve
 * @rval: Where the resulting IPv6 address descriptor should be stored
 * @return: NULL if it succeeded, or a pointer towards
 *          a string describing the error if any.
 *          (const char* means the caller cannot modify or free the return value,
 *           so do not use malloc!)
 */
const char * real_address(const char *address, struct sockaddr_in6 *rval){
  int err;
  if(rval==NULL){
    fprintf(stderr, "rval==NULL\n");
    return ("rval is NULL");
  }
  struct addrinfo hints, *servinfo;
  hints.ai_family=AF_INET6;
  hints.ai_socktype=SOCK_DGRAM;
  hints.ai_protocol=17;
  hints.ai_flags=(AI_V4MAPPED | AI_ADDRCONFIG);
  err=getaddrinfo(address, NULL, &hints, &servinfo);
  //Testing returned error
  if(err!=0){
    fprintf(stderr, "Err at getaddrinfo:%d \n",err);
    fprintf(stderr, "%s\n",gai_strerror(err));
    return gai_strerror(err);
  }
  struct sockaddr_in6 *t=(struct sockaddr_in6 *)(hints.ai_addr);
    *rval=*t;
    //Free servinfo

    freeaddrinfo(servinfo);
    return NULL;
}
