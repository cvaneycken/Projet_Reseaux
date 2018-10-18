#include <stdlib.h> /* EXIT_X */
#include <stdio.h> /* fprintf */
#include <unistd.h> /* getopt */


#include "functions_library.h"

int main(int argc, char *argv[])
{
	char *host;
	int port = -1;
	//int opt; //TO delet if switch(opt) is not used
	int i=0;
	int f = 0; //flag determining if the -f option is activated
	char * file;// file to which data is sent

	//Check for the facultative -f argument
	/*while ((opt = getopt(argc, argv, "f")) != -1) {
		switch (opt) {
      case 'f':
				//f = 1;
        printf("you chose f with %s\n", optarg);
        break;
    }
  }*/

	//Retrieve the host name and port number from arguments
	for(i=0;i<argc;i++){
    if(strcmp(argv[i],"::1")==0){
      host=argv[i];
			port=atoi(argv[i+1]);
    }
		else if(strcmp(argv[i],"::")==0)
		{
			//Listening to all ports
		}
		else if(strcmp(argv[i],"-f")==0)
		{
			f=1;
			file=argv[i+1];
			printf("you chose f with %s, f = %d\n", file,f);
		}
  }
	if(port == -1)
	{
		fprintf(stderr,"No argument matches a valid hostname");
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
	/* Creation of sender socket deleted
	sfd = create_socket(NULL, -1, &addr, port); // Connected */
	sfd = create_socket(&addr, port, NULL, -1); // Bound
	if (sfd > 0 && wait_for_client(sfd) < 0) { // Connected
	fprintf(stderr,
			"Could not connect the socket after the first message.\n");
	close(sfd);
	return EXIT_FAILURE;
	}

	if (sfd < 0) {
		fprintf(stderr, "Failed to create the socket!\n");
		return EXIT_FAILURE;
	}
	/* Process I/O */
	read_write_loop(sfd);

	close(sfd);

	return EXIT_SUCCESS;
}
