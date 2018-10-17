#include <stdlib.h> /* EXIT_X */
#include <stdio.h> /* fprintf */
#include <unistd.h> /* getopt */
#include <string.h>
#include "real_address.h"
#include "create_socket.h"
#include "read_write_loop.h"
#include "wait_for_client.h"

int main(int argc, char *argv[])
{
	char *host;
	int port;
	int opt;
	int i=0;
	while ((opt = getopt(argc, argv, "f")) != -1) {
		switch (opt) {
      case 'f':
        printf("you chose f with %s\n", optarg);
        break;
    }
  }
  for(i=0;i<argc;i++){
    if(strcmp(argv[i],"::1")==0){
      host="::";
			port=atoi(argv[i+1]);
    }
  }
	//sender is client
	int client = 1;
	printf("%d %s\n",argc,*argv);
	/* Resolve the hostname */
	struct sockaddr_in6 addr;
	const char *err = real_address(host, &addr);
	if (err) {
		fprintf(stderr, "Could not resolve hostname %s: %s\n", host, err);
		return EXIT_FAILURE;
	}
	/* Get a socket */
	int sfd;
	if (client) {
		sfd = create_socket(NULL, -1, &addr, port); /* Connected */
	} else {
		sfd = create_socket(&addr, port, NULL, -1); /* Bound */
		if (sfd > 0 && wait_for_client(sfd) < 0) { /* Connected */
			fprintf(stderr,
					"Could not connect the socket after the first message.\n");
			close(sfd);
			return EXIT_FAILURE;
		}
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
