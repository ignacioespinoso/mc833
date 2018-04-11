// Server Code
// Reference:   https://www.programminglogic.com/example-of-client-server-program-in-c-using-sockets-and-tcp/
//              https://www.thegeekstuff.com/2011/12/c-socket-programming/
/*  Giovani Nascimento Pereira - 168609
    Ignacio Espinoso Ribeiro - 169767

    MC833 - 2S2018
    Unicamp
*/

#include "libraries.h"  // file with the connection imports
#include "Data.c"       // database file and functions

#define PORT "3490"     // the port users will be connecting to
#define BACKLOG 10      // how many pending connections queue will hold

/// Created methods
bool sendMessageToClient(int fd, char *message);
bool receiveMessageFromClient(int sockfd, char* buffer);
bool newConnectionServerLoop(int client_fd);
bool checkReceivedMessage(char *message, char *answer);
void getCodeFromRequest(char *request, char *code);
void getCommentFromRequest(char *request, char *comment);

// Global variable to get system Time
connectionTime op;

void sigchld_handler(int s) {
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;
    while(waitpid(-1, NULL, WNOHANG) > 0);
    errno = saved_errno;
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void) {

    // Connection variables
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while(1) {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);
        printf("server: got connection from %s\n", s);

        if (!fork()) { // this is the child process
            close(sockfd); // child doesn't need the listener

            // Send the "Connected message!"
            if (sendMessageToClient(new_fd, "Connected") == false)
                perror("send");

            if (newConnectionServerLoop(new_fd) == false){
                perror("Connection Failed");
                exit(1);
            }

            close(new_fd);
            exit(0);
        }
        close(new_fd);  // parent doesn't need this
    }

    return 0;
}

/// Send a message to the client
/// Param:  fd - the client connection number (?)
///         message - a String (char*) for the sending message
/// RETURN: bool -  true if the message was sent correctly
///                 false if something went wrong
bool sendMessageToClient(int fd, char *message){
    //Get send Time
    gettimeofday(&op.sendTime, NULL);
    if (send(fd, message, strlen(message), 0) == -1){
        return false;
    }
    printf("server: Msg being sent: %s [Number of bytes sent: %lu]\n",message, strlen(message));

    //Printing time execution interval
    printExecutionTimeServer(op);
    return true;
}

/// Waits to receive a message from the client
/// PARAM:  sockfd - the client connection idetifier
///         buffer - a char* to insert the message
/// RETURN: bool   - true if the message was received correclty
///                  false if something went wrong
bool receiveMessageFromClient(int sockfd, char* buffer){
    int num;
    num = recv(sockfd, buffer, 1024, 0);

    // Get receive time
    gettimeofday(&op.receiveTime, NULL);

    if (num <= 0)
        return false;
    // if num == 0 connection closed (?)
    buffer[num] = '\0';
    printf("\nserver: Message Received From Client -  %s\n", buffer);
    return true;
}

/// After being connected to a client
/// this method can be called to continue the connection
/// PARAM:  client_fd - the client connection idetifier
/// RETURN: bool -  true if the connection was closes properly
///                 false if something went wrong
bool newConnectionServerLoop(int client_fd){

    int num;
    char buffer[MAXDATASIZE];
    char answer[MAXDATASIZE];

    // Stays in loop until the connection is closed
    while(1) {

        if (receiveMessageFromClient(client_fd, buffer) == false) {
            printf(">>> There was an error or the connection was closed\n\n");
            break;
        }

        // Check the request
        checkReceivedMessage(buffer, answer);

        if (sendMessageToClient(client_fd, answer) == false) {
            fprintf(stderr, "Failure Sending Message\n");
            close(client_fd);
            break;
        }
    }
    return true;
}

/// Check an incomming request
/// to get the proper answer from the Data.c
/// PARAM:  message - the incoming request
///         answer  - the answer string
/// Return: true    - the correct answer could be found
///         false   - the answer was not found
bool checkReceivedMessage(char *message, char *answer){

    int request;
    char code[6]; // The subject code has a limited size
    char comment[200];
    request = message[0] - '0';

    // Set the current operation on the timeConnection values
    op.operation = request;

    switch (request) {
        case 1:
            strcpy(answer, "All Subjects:\n");
            strcat(answer, getCodigosNomesDisciplinas());
            break;
        case 2:
            getCodeFromRequest(message, code);
            strcpy(answer, code);
            strcat(answer, "\n");
            strcat(answer, getEmentaFromCodigo(code));
            break;
        case 3:
            getCodeFromRequest(message, code);
            strcpy(answer, code);
            strcat(answer, "\n");
            strcat(answer, getInformacaoDisciplinaFromCodigo(code));
            break;
        case 4:
            getCodeFromRequest(message, code);
            strcpy(answer, code);
            strcat(answer, "\n");
            strcat(answer, getComentarioFromCodigo(code));
            break;
        case 5:
            strcpy(answer, "All Subjects:\n");
            strcat(answer, getAllInfo());
            break;
        case 6:
            getCodeFromRequest(message, code);
            strcpy(answer, "Next class ");
            strcat(answer, code);
            strcat(answer, ": ");
            getCommentFromRequest(message, comment);
            strcat(answer, comment);
            if (setComentarioForCode(code, comment) == false){
                strcpy(answer, "The comment couldnt be written. Check the subject code\n");
            }
            break;
        default:
            strcpy(answer, message);
            strcat(answer, " [Unrecognized Message/Request]");
            break;
    }

    return false;
}

/// Extract the code from a request of the type
/// "<int Number> <5 character subject code>"
/// PARAM:  request - String (char*) with the complete request text
///         code    - the return of the collected code
void getCodeFromRequest(char *request, char *code){
    for (int i=0; i<6; i++){
        code[i] = request[i+2];
    }
    code[5] = 0;
}

/// Extract the comment from a request of the type
/// "<int Number> <5 character subject code> <message request>"
/// PARAM:  request - String (char*) with the complete request text
///         code    - the return of the collected code
void getCommentFromRequest(char *request, char *comment){
    int i =0;
    do {
        comment[i] = request[i+8];
        i++;
    } while(request[i+8] != '\0');
    comment[i] = '\0';
}
