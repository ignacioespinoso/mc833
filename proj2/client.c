// UDP Client Code
// Reference:   https://www.binarytides.com/programming-udp-sockets-c-linux/
/*  Giovani Nascimento Pereira - 168609
    Ignacio Espinoso Ribeiro - 169767

  MC833 - 2S2018
  Unicamp
*/

#include "libraries.h"

#define BUFLEN 2000  //Max length of buffer
#define PORT 8888   //The port on which to send data

// Creating Bool type
typedef int bool;
enum bool {false, true};

// Auxiliar Methods
bool selectRequestMessage(char *request);
void die(char *s);
void checkInformation(int numberOfParam);
void checkTestMode(int sock, int argc, char** argv, struct sockaddr_in si_other, unsigned int slen);
void executeTestMode(int sock, struct sockaddr_in si_other, unsigned int slen);
bool sendMessageToServer(char *message, struct sockaddr_in si_other, unsigned int slen, int sock);
bool receiveMessageFromServer(int sock, char* buffer, struct sockaddr_in si_other, unsigned int slen);

// Global variable for time manangement
connectionTime op;

int main(int argc, char *argv[]) {
    struct sockaddr_in si_other;
    int sock;
    unsigned int slen=sizeof(si_other);
    char buf[BUFLEN];
    char message[BUFLEN];


    // Get server IP address
    checkInformation(argc);
    char *server = argv[1];

 	// Create UDP socket
    if ( (sock=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        die("socket");
    }

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);

    if (inet_aton(server , &si_other.sin_addr) == 0) {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    checkTestMode(sock, argc, argv, si_other, slen);

 	// Message Loop
    while(1) {

        // Check if the user tried to disconnect
        // or get the next servre request
        if (selectRequestMessage(message) == false) {
        	break;
        }

        // Send message
        if (sendMessageToServer(message, si_other, slen, sock) == false) {
        	die("sendto()");
        }

        // Receive message
        if (receiveMessageFromServer(sock, buf, si_other, slen) == false) {
        	die("recvfrom()");
        }


        //Make a little stop
        printf("\n(Press Enter to continue)\n\n");
        getchar();
        getchar();
    }

    close(sock);
    return 0;
}

/// Sends a string message to the server
/// RETURN: bool -  true if the message was sent
///                 false if something went wrong
bool sendMessageToServer(char *message, struct sockaddr_in si_other, unsigned int slen, int sock){
    //Record send Time
	gettimeofday(&op.sendTime, NULL);
	//Send the message
    if (sendto(sock, message, strlen(message), 0, (struct sockaddr *) &si_other, slen)==-1) {
        return false;
    }
    return true;
}

/// Waits to receive a message from the server
/// RETURN: bool -  true if the message was received correclty
///                 false if something went wrong
bool receiveMessageFromServer(int sock, char* buffer, struct sockaddr_in si_other, unsigned int slen){
    int n;
    fd_set read_fds, write_fds;
    // Sets timeout values.
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    FD_ZERO(&read_fds);
    FD_SET(sock, &read_fds);
    n = select(sock+1, &read_fds, &write_fds, 0, &tv);
    if(n < 0)
    {
        perror("ERROR Server : select()\n");
        close(sock);
        exit(1);
    }
    //try to receive some data, this is a blocking call
    if(FD_ISSET(sock, &read_fds)) {
        if (recvfrom(sock, buffer, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1) {
            return false;
        }
        // Get the received time
        gettimeofday(&op.receiveTime, NULL);

        FD_CLR(sock, &read_fds);
    }

    puts(buffer);

    // Print the time
    printConnectionTimeClient(op);

    return true;
}

/// Here we need to make a request to our server
/// the user will select from a list of controlled requests
/// PARAM:  the string where the request will be written
bool selectRequestMessage(char *request){

    int option;

    printf("Make a Request or send a Message:\n\n");
    printf("1 - Get all subjects\n");
    printf("2 - Get subject description\n");
    printf("3 - Get subject full information\n");
    printf("4 - Get next class information\n");
    printf("5 - Get all subjects info\n");
    // Teacher only:
    printf("6 - Set nextClassComment\n");
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
        case 6:
            strcpy(request, "6 ");
            char change[200];

            //Verifies if the user is a teacher.
            char verification[5];
            printf("WARNING: Only teachers may set comments. Are you a teacher? y/n  ");
            scanf(" %s", verification);
            if(strcmp(verification, "y") != 0) {
                printf("You are not a teacher! Aborting operation.\n");
                return false;
            }

            printf("Ok! Proceeding operation...\n");
            // Get the subject
            printf("Type the subject you want to change next class commentary: ");
            scanf(" %s", change);
            strcat(request, change);
            strcat(request, " ");

            // Get the next calss comment
            printf("Type the new Commentary: ");
            scanf(" ");
            fgets(change, 200, stdin);
            strcat(request, change);

            printf("MESSAGE: %s", request);
            return true;
        case 7:
            printf("Type your message: ");
            strcpy(request, "6 ");
            return true;
        case 9:
            printf("Logging as a teacher...");
            strcpy(request, "9 ");
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

/// Default error method
/// print error message and kill the program
void die(char *s) {
    perror(s);
    exit(1);
}

/// Check if the user typed the hostname
void checkInformation(int numberOfParam){
    if (numberOfParam < 2) {
        fprintf(stderr,"usage: type in the client hostname\n");
        exit(1);
    }
}

/// Check if the user run the program on TEST MODE
///
void checkTestMode(int sock, int argc, char** argv, struct sockaddr_in si_other, unsigned int slen){
    if (argc > 2){
        if (strcmp(argv[2], "TEST") == 0){
            executeTestMode(sock, si_other, slen);
        }
    }
}

/// This method is used on TEST MODE
/// to make 50 connections of all types to get the current
/// execution and connection time
void executeTestMode(int sock, struct sockaddr_in si_other, unsigned int slen){

    char buf[BUFLEN];

    printf("-------- TEST MODE ---------\n\n");

    printf(">>>>>>> Category 1 Messages:\nGet all subjects and names\n\n");
    op.operation = 1;
    for (int i=0; i<50; i++){
        if (sendMessageToServer("1 Get all Subjects", si_other, slen, sock) == false) {
            close(sock);
            break;
        }

        //The message was sent correctly - waiting for answer
        if (receiveMessageFromServer(sock, buf, si_other, slen) == false) {
            printf("Either Connection Closed or Error\n");
            break;
        }
    }

    printf(">>>>>>> Category 2 Messages:\nGet subject description\n\n");
    op.operation = 2;
    for (int i=0; i<50; i++){
        if (sendMessageToServer("2 MC102", si_other, slen, sock) == false) {
            close(sock);
            break;
        }

        //The message was sent correctly - waiting for answer
        if (receiveMessageFromServer(sock, buf, si_other, slen) == false) {
            printf("Either Connection Closed or Error\n");
            break;
        }
    }

    printf(">>>>>>> Category 3 Messages:\nGet subject full information\n\n");
    op.operation = 3;
    for (int i=0; i<50; i++){
        if (sendMessageToServer("3 MA111", si_other, slen, sock) == false) {
            close(sock);
            break;
        }

        //The message was sent correctly - waiting for answer
        if (receiveMessageFromServer(sock, buf, si_other, slen) == false) {
            printf("Either Connection Closed or Error\n");
            break;
        }
    }

    printf(">>>>>>> Category 4 Messages:\nGet nex class information\n\n");
    op.operation = 4;
    for (int i=0; i<50; i++){
        if (sendMessageToServer("4 EE532", si_other, slen, sock) == false) {
            close(sock);
            break;
        }

        //The message was sent correctly - waiting for answer
        if (receiveMessageFromServer(sock, buf, si_other, slen) == false) {
            printf("Either Connection Closed or Error\n");
            break;
        }
    }

    printf(">>>>>>> Category 5 Messages:\nGet all Subjects info\n\n");
    op.operation = 5;
    for (int i=0; i<50; i++){
        if (sendMessageToServer("5 Get all subjects info", si_other, slen, sock) == false) {
            close(sock);
            break;
        }

        //The message was sent correctly - waiting for answer
        if (receiveMessageFromServer(sock, buf, si_other, slen) == false) {
            printf("Either Connection Closed or Error\n");
            break;
        }
    }

    printf(">>>>>>> Category 6 Messages:\nWrite new Next Class information\n\n");
    op.operation = 6;
    for (int i=0; i<50; i++){
        if (sendMessageToServer("6 EE532 Trocando esse comentario por esse daqui", si_other, slen, sock) == false) {
            close(sock);
            break;
        }

        //The message was sent correctly - waiting for answer
        if (receiveMessageFromServer(sock, buf, si_other, slen) == false) {
            printf("Either Connection Closed or Error\n");
            break;
        }
    }

    printf("\n\n-------- TEST MODE FINISHED ---------\n");
    printf("check the Log file to get the system time easily\n\n");

    //Finish connection and stop the program
    close(sock);
    exit(0);
}
