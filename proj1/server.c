#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define MYPORT "3490"  // the port users will be connecting to
#define BACKLOG 5     // how many pending connections queue will hold

int main() {
    struct addrinfo hints, *servinfo;
    memset(&hints, 0, sizeof(hints)); // make sure the struct is empty
    struct sockaddr_storage their_addr; // connector's address information
    hints.ai_family = AF_UNSPEC; //don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; //TCP stream sockets
    hints.ai_flags = AI_PASSIVE; // fill in my IP for me

    //Sets things up
    int status;
    if ((status = getaddrinfo (NULL, MYPORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    //Creates socket
    int server_socket = socket(servinfo->ai_family, servinfo->ai_socktype
                                , servinfo->ai_protocol);
    //Binds port to socket
    int server_port;
    server_port = bind(server_socket, servinfo->ai_addr, servinfo->ai_addrlen);
    if(server_port < 0) {
        perror("bind call error");
    }
    //Listens and accepts connection
    listen(server_socket, BACKLOG);
    socklen_t addr_size;
    int new_connection;

    printf("server: waiting for connections...\n");
    while(1) {
        addr_size = sizeof their_addr;
        new_connection = accept(server_socket, (struct sockaddr *)&their_addr
                                , &addr_size);
        if (new_connection == -1) {
           perror("accept error");
           continue;
        }
        //Gets information about the peer.
        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);

        if (!fork()) { // this is the child process
            close(new_connection); // child doesn't need the listener
            if (send(new_connection, "Hello, world!", 13, 0) == -1)
                perror("send error");
            close(new_fd);
            exit(0);
        }
        close(new_connection);  // parent doesn't need this
    }
  return 0;
}
