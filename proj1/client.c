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

// Creating Bool type
typedef int bool;
enum bool {false, true};

// Global variable for time manangement
connectionTime op;

// Created methods to handle connection
bool newConnectionClientLoop(int sockfd);
bool receiveMessageFromServer(int sockfd, char* buffer);
bool receiveMessageFromServer(int sockfd, char* buffer);
bool selectRequestMessage(char *request);
void *get_in_addr(struct sockaddr *sa);
void checkInformation(int numberOfParam);
void checkTestMode(int sockfd, int argc, char** argv);
void executeTestMode(int sockfd);

int main(int argc, char *argv[]) {
    int sockfd, numbytes = 0;  
    char buf[MAXDATASIZE];
    addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    // CHeck if the code was executed correctly
    checkInformation(argc);

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

    if (receiveMessageFromServer(sockfd, buf) == false) {
        perror("error on recieving connect message");
        exit(1);
    }

    // Check if the user run the program on automatic TEST MODE
    checkTestMode(sockfd, argc, argv);

    // Connection loop - Connection already stablished
    if (newConnectionClientLoop(sockfd) == false){
        perror("Connection error");
        exit(1);
    }

    // End connection
    close(sockfd);

    return 0;
}

/// Get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

/// Check if the user typed the hostname
void checkInformation(int numberOfParam){
    if (numberOfParam < 2) {
        fprintf(stderr,"usage: type in the client hostname\n");
        exit(1);
    } 
}

/// Sends a string message to the server
/// PARAM:  fd - the server connection identifier
///         message - the string message
/// RETURN: bool -  true if the message was sent
///                 false if something went wrong
bool sendMessageToServer(int fd, char *message){
    if ((send(fd, message, strlen(message),0))== -1){
        return false;
    }
    //Record send Time
    gettimeofday(&op.sendTime, NULL);
    printf("client: Message being sent: %s\n",message);
    return true;
}

/// Waits to receive a message from the server
/// PARAM:  sockfd - the server connection idetifier
///         buffer - a char* to insert the message
/// RETURN: bool -  true if the message was received correclty
///                 false if something went wrong
bool receiveMessageFromServer(int sockfd, char* buffer){
    int num;
    num = recv(sockfd, buffer, MAXDATASIZE-1, 0);
    if (num < 0)
        return false;
    // Get the recieved time
    gettimeofday(&op.recieveTime, NULL);
    buffer[num] = '\0';
    printf("client: Message Received From Server:\n%s\n",buffer);

    // Print the time
    printConnectionTimeClient(op);

    return true;
}

/// After being connected to a server
/// this method can be called to continue the connection
/// so it will only closes when thi method comes to an end
/// PARAM:  sockfd - the server connection idetifier
/// RETURN: bool -  true if the connection was closes properly
///                 false if something went wrong
bool newConnectionClientLoop(int sockfd){

    char buffer[MAXDATASIZE];

    // Stays in loop until user close the connection
    while(1) {

        //Show the menu to select a message to server
        if (selectRequestMessage(buffer) == false){
            // The user decided to close the connection
            close(sockfd);
            return true;
        }
        if (sendMessageToServer(sockfd, buffer) == false) {
            close(sockfd);
            return false;
        }

        //The message was sent correctly - waiting for answer
        if (receiveMessageFromServer(sockfd, buffer) == false) {
            printf("Either Connection Closed or Error\n");
            return false;
        }

        //Make a little stop
        printf("\n(Press Enter to continue)\n\n");
        getchar();
        getchar();
    }

    return true;
}

/// Here we need to make a request to our server
/// the user will select from a list of controlled requests
/// PARAM:  the string where the request will be written
bool selectRequestMessage(char *request){

    int option; 

    printf("Connected to GioBaiano Server\nMake a Request or send a Message:\n\n");
    printf("1 - Get all subjects\n");
    printf("2 - Get subject description\n");
    printf("3 - Get subject full information\n");
    printf("4 - Get next class information\n");
    printf("5 - Get all subjects info\n");
    // Teacher only:
    //printf("6 - Set nextClassComment\n");
    printf("7 - Send custom message\n");
    printf("0 - Close Connection\n");

    // Get user command
    scanf(" %d", &option);
    // Save the request on the time operation
    op.operation = option;

    switch (option){
        case 0:
            // Close connection
            return false;
        case 1:
            strcpy(request, "1 Get all subjects");
            return true;
        case 2:
            strcpy(request, "2 ");
            break;
        case 3:
            strcpy(request, "3 ");
            break;
        case 4:
            strcpy(request, "4 ");
            break;
        case 5:
            strcpy(request, "5 Get all subjects info");
            return true;
        // TODO case 6
        case 7:
            printf("Type your message: ");
            scanf(" %s", request);
            return true;
        default:
            printf("Invalid request... Closing connection\n");
            return false;
    }

    char code[10];
    printf("For which Subject [Type subject Code]: ");
    scanf(" %s", code);
    strcat(request, code);

    return true;
}

/// Check if the user run the program on TEST MODE
///
void checkTestMode(int sockfd, int argc, char** argv){
    if (argc > 2){
        if (strcmp(argv[2], "TEST") == 0){
            executeTestMode(sockfd);
        }
    }
}

/// This method is used on TEST MODE
/// to make 50 connections of all types to get the current 
/// execution and connection time
void executeTestMode(int sockfd){

    char buffer[MAXDATASIZE];

    printf("-------- TEST MODE ---------\n\n");

    printf(">>>>>>> Category 1 Messages:\nGet all subjects and names\n\n");
    op.operation = 1;
    for (int i=0; i<50; i++){
        if (sendMessageToServer(sockfd, "1 Get all subjects") == false) {
            close(sockfd);
            break;
        }

        //The message was sent correctly - waiting for answer
        if (receiveMessageFromServer(sockfd, buffer) == false) {
            printf("Either Connection Closed or Error\n");
            break;
        }
    }

    printf(">>>>>>> Category 2 Messages:\nGet subject description\n\n");
    op.operation = 2;
    for (int i=0; i<50; i++){
        if (sendMessageToServer(sockfd, "2 MC102") == false) {
            close(sockfd);
            break;
        }

        //The message was sent correctly - waiting for answer
        if (receiveMessageFromServer(sockfd, buffer) == false) {
            printf("Either Connection Closed or Error\n");
            break;
        }
    }

    printf(">>>>>>> Category 3 Messages:\nGet subject full information\n\n");
    op.operation = 3;
    for (int i=0; i<50; i++){
        if (sendMessageToServer(sockfd, "3 MA111") == false) {
            close(sockfd);
            break;
        }

        //The message was sent correctly - waiting for answer
        if (receiveMessageFromServer(sockfd, buffer) == false) {
            printf("Either Connection Closed or Error\n");
            break;
        }
    }

    printf(">>>>>>> Category 4 Messages:\nGet nex class information\n\n");
    op.operation = 4;
    for (int i=0; i<50; i++){
        if (sendMessageToServer(sockfd, "4 EE532") == false) {
            close(sockfd);
            break;
        }

        //The message was sent correctly - waiting for answer
        if (receiveMessageFromServer(sockfd, buffer) == false) {
            printf("Either Connection Closed or Error\n");
            break;
        }
    }

    printf(">>>>>>> Category 5 Messages:\nGet all Subjects info\n\n");
    op.operation = 5;
    for (int i=0; i<50; i++){
        if (sendMessageToServer(sockfd, "5 Get all subjects info") == false) {
            close(sockfd);
            break;
        }

        //The message was sent correctly - waiting for answer
        if (receiveMessageFromServer(sockfd, buffer) == false) {
            printf("Either Connection Closed or Error\n");
            break;
        }
    }

    printf("\n\n-------- TEST MODE FINISHED ---------\n");
    printf("check the Log file to get the system time easily\n\n");

    //Finish connection and stop the program
    close(sockfd);
    exit(0);
}