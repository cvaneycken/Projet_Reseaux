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
  while(1){
    int activity=
  }
}
