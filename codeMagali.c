#include <netinet/in.h> /* * sockaddr_in6 */
#include <sys/types.h> /* sockaddr_in6 */
#include <string.h> /* NULL */
#include <netdb.h> /* struct addrinfo */
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <arpa/inet.h>

/* Resolve the resource name to a usable IPv6 address
 * @address: The name to resolve
 * @rval: Where the resulting IPv6 address descriptor should be stored
 * @return: NULL if it succeeded, or a pointer towards
 *          a string describing the error if any.
 *          (const char* means the caller cannot modify or free the return value,
 *           so do not use malloc!)
 */
const char * real_address(const char *address, struct sockaddr_in6 *rval)
{
  struct addrinfo *result;
  struct addrinfo criteria;
  criteria.ai_family = AF_INET6;
  criteria.ai_socktype = 0;
  criteria.ai_protocol = 0;
  int err;
  err = getaddrinfo(address, NULL, &criteria, &result);
  if(err == 0)
  {
    char *error = "Problème à l'exécution de getaddrinfo";
    return error;
  }
  rval = (struct sockaddr_in6 *)result;
  /* Ces deux lignes de code sont inutiles mais les tests INGInious ne compilent pas si rval n'est pas utilisée
  uint32_t a = rval->sin6_flowinfo;
  a++;
  */
  return NULL;
}
