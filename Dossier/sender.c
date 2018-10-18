#include <stdlib.h> /* EXIT_X */
#include <stdio.h> /* fprintf */
#include <unistd.h> /* getopt */
#include <string.h>
#include "functions_library.h"


int main(int argc, char *argv[])
{
	char *host;
	int port = -1;
	//int opt; //TO delet if switch(opt) is not used
	int i=0;
	int f=0; //flag determining if the -f option is activated
	char * file;// file from which data is read

	/*//Check for the facultative -f argument
	while ((opt = getopt(argc, argv, "f")) != -1) {
		switch (opt) {
      case 'f':
				f = 1;
        printf("you chose f with %s\n", optarg);
        break;
    }
  }*/ // Done with strcmp

	//Retrieve host name, port number, [file] from arguments
  for(i=0;i<argc;i++){
    if(strcmp(argv[i],"::1")==0){
      host=argv[i];
			port=atoi(argv[i+1]);
    }
		else if(strcmp(argv[i],"-f")==0)
		{
			f=1;
			file=argv[i+1];
		}
  }
	if(port == -1)
	{
		fprintf(stderr,"No argument matches a valid hostname\n");
		return EXIT_FAILURE;
	}

	/* Resolve the hostname */
	struct sockaddr_in6 addr;
	const char *err = real_address(host, &addr);
	if (err) {
		fprintf(stderr, "Could not resolve hostname %s: %s\n", host, err);
		return EXIT_FAILURE;
	}
	/* Get a socket */
	int sfd;
	sfd = create_socket(NULL, -1, &addr, port); /* Connected */
	if (sfd < 0) {
		fprintf(stderr, "Failed to create the socket!\n");
		return EXIT_FAILURE;
	}
	/* Process I/O */
	//read_write_loop(sfd);

	ssize_t nBytes;
	fd_set fds;
	char buffer[1024]; //------ Pk cette valeur là?
	struct timeval timeVal =(struct timeval){
		.tv_sec = 5,
		.tv_usec =0,
	};

	//selection of input depending of -f or not
	int input;
	if(f)
	{
		//open input for file
		printf("input file is %s\n",file);
	}
	else
	{
		printf("input is %d\n",input);
		input = STDIN_FILENO;
	}
	//Send/listen loop
	while(1){
		FD_ZERO(&fds);
		FD_SET(sfd,&fds);
		FD_SET(input, &fds);

		/*if(select(FD_SETSIZE, &fds, NULL, NULL, &timeVal) == -1){ //checks for available elements on input
			fprintf(stderr, "Err: select function, %s\n",strerror(errno));
			exit(-1);
		}*/ //Previous place of this
		if(FD_ISSET(sfd,&fds)){
			nBytes=read(sfd,buffer,sizeof(buffer));
			if(nBytes==0)
			{
				printf("nBytes==0\n");
				return -1;
			}
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

		//Reading from input to write on socket
	 if(FD_ISSET(input,&fds)){
		 nBytes=read(input,buffer,sizeof(buffer));
		 if(nBytes==0)
		 {
			 printf("nBytes==0\n");
			 return -1;
		 }
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

	close(sfd);

	return EXIT_SUCCESS;
}
