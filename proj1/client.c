// Client Code
// Reference:   https://www.programminglogic.com/example-of-client-server-program-in-c-using-sockets-and-tcp/
//              https://www.thegeekstuff.com/2011/12/c-socket-programming/
/*  Giovani Nascimento Pereira - 168609
  Ignacio Espinoso Ribeiro - 

  MC833 - 2S2018
  Unicamp
*/

#include "libraries.h"

// int main(int argc, char *argv[]) {
//     int sockfd = 0, n = 0;
//     char recvBuff[1024];
//     struct sockaddr_in serv_addr; 

//     if(argc != 2) {
//         printf("\n Usage: %s <ip of server> \n",argv[0]);
//         return 1;
//     } 

//     memset(recvBuff, '0',sizeof(recvBuff));
//     if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
//         printf("\n Error : Could not create socket \n");
//         return 1;
//     } 

//     memset(&serv_addr, '0', sizeof(serv_addr)); 

//     serv_addr.sin_family = AF_INET;
//     serv_addr.sin_port = htons(5000); 

//     if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0) {
//         printf("\n inet_pton error occured\n");
//         return 1;
//     } 

//   printf("CONNECTING TO %s\n", argv[1]);
//     if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
//        printf("\n Error : Connect Failed \n");
//        return 1;
//     }
//     printf("CONNECTED\n");

//     while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0) {
//         recvBuff[n] = 0;
//         if(fputs(recvBuff, stdout) == EOF)
//         {
//             printf("\n Error : Fputs error\n");
//         }
//     } 

//     if(n < 0) {
//         printf("\n Read error \n");
//     } 

//     return 0;
// }

#define PORT "3490" // the port client will be connecting to 

#define MAXDATASIZE 100 // max number of bytes we can get at once 


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
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

    close(sockfd);

    return 0;
}