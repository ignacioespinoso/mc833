// Client Code
// Reference:   https://www.programminglogic.com/example-of-client-server-program-in-c-using-sockets-and-tcp/
//              https://www.thegeekstuff.com/2011/12/c-socket-programming/
/*  Giovani Nascimento Pereira - 168609
  Ignacio Espinoso Ribeiro - 

  MC833 - 2S2018
  Unicamp
*/

#include "libraries.h"

#define PORT "3490" // the port client will be connecting to 
#define MAXDATASIZE 2000 // max number of bytes we can get at once 

// Creating Bool type
typedef int bool;
enum bool {false, true};

// Created methods to handle connection
bool newConnectionClientLoop(int sockfd);

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[]) {
    int sockfd, numbytes;  
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }
        
        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure

    if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }

    buf[numbytes] = '\0';
    printf("client: received '%s'\n",buf);
    // Connection loop - Connection already stablished
    if (newConnectionClientLoop(sockfd) == false){
        perror("Connection error");
        exit(1);
    }

    // End connection
    close(sockfd);

    return 0;
}

bool sendMessageToServer(int fd, char *message){
    if ((send(fd, message, strlen(message),0))== -1){
        return false;
    }
    return true;
}

bool newConnectionClientLoop(int sockfd){

    int num;
    char buffer[MAXDATASIZE];

    // Stays in loop until user close the connection
    while(1) {
        printf("Client: Enter Data for Server:\n");
        fgets(buffer,MAXDATASIZE-1,stdin);
        if (sendMessageToServer(sockfd, buffer)) {
            close(sockfd);
            return false;

        } else {
            printf("Client: Message being sent: %s\n",buffer);
            num = recv(sockfd, buffer, sizeof(buffer),0);
            if ( num <= 0 ) {
                printf("Either Connection Closed or Error\n");
                //Break from the While
                break;
            }

            buffer[num] = '\0';
            printf("Client:Message Received From Server -  %s\n",buffer);
        }
    }

    return true;
}