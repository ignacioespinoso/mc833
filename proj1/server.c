#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>

int getaddrinfo(const char *node, const char *service,
const struct addrinfo *hints, struct addrinfo **res);
int main() {
  //Discovers info about server's own address
  getaddrinfo(NULL, "3490",);
  //Creates socket
  int server_socket = socket(PF_INET, SOCK_STREAM, 0);
  //Binds port to socket
  int server_port = bind(server_socket, ,);
  //Listens
  //Do stuff
  return 0;
}
