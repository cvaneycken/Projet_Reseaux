
#include "functions_library.h"
#define _OPEN_SYS_SOCK_IPV6 1

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

const char * real_address(const char *address, struct sockaddr_in6 *rval){
  int err;
  if(rval==NULL){
    fprintf(stderr, "rval==NULL\n");
    return ("rval is NULL");
  }
  struct addrinfo hints, *servinfo;
  hints.ai_family=AF_INET6;
  hints.ai_socktype=SOCK_DGRAM;
  hints.ai_protocol=IPPROTO_UDP;
  hints.ai_flags=AI_PASSIVE;
  err=getaddrinfo(address, NULL, &hints, &servinfo);
  //Testing returned error
  if(err!=0){
    fprintf(stderr, "Err at getaddrinfo:%d \n",err);
    fprintf(stderr, "%s\n",gai_strerror(err));
    return gai_strerror(err);
  }
  memcpy((void *)rval,(void*)servinfo->ai_addr,servinfo->ai_addrlen);
    //Free servinfo

    freeaddrinfo(servinfo);
    return NULL;
}

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
// INSPIRED FROM mrahrauld's github

void read_write_loop(int sfd)
{
    ssize_t nBytes;
    fd_set fds;
    char buffer[1024]; //------ Pk cette valeur là?
    struct timeval timeVal =(struct timeval){
      .tv_sec = 5,
      .tv_usec =0,
    };
    while(1){
      FD_ZERO(&fds);
      FD_SET(sfd,&fds);
      FD_SET(STDIN_FILENO, &fds);

      /*if(select(FD_SETSIZE, &fds, NULL, NULL, &timeVal) == -1){ //checks for available elements on input
        fprintf(stderr, "Err: select function, %s\n",strerror(errno));
        exit(-1);
      }*/ //Previous place of this
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

      //checking for available elements on input
      if(select(FD_SETSIZE, &fds, NULL, NULL, &timeVal) == -1){
        fprintf(stderr, "Err: select function, %s\n",strerror(errno));
        exit(-1);
      }

      //Reading from stdin to write on socket
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
//SOURCE: https://github.com/sCreami/cnp3-p1/blob/master/inginious/ta2/read_write_loop.c
