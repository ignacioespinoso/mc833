// UDP Server Code
// Reference:   https://www.binarytides.com/programming-udp-sockets-c-linux/
/*  Giovani Nascimento Pereira - 168609
    Ignacio Espinoso Ribeiro - 169767

  MC833 - 2S2018
  Unicamp
*/

#include "libraries.h"  // file with the connection imports
#include "Data.c"       // database file and functions
 
#define BUFLEN 2000  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

// Global variable to get system Time
connectionTime op;

// Auxiliar Methods
bool checkReceivedMessage(char *message, char *answer, int* usr_type);
void getCodeFromRequest(char *request, char *code);
void getCommentFromRequest(char *request, char *comment);
void die(char *s);
 
int main(void) {
    struct sockaddr_in si_me, si_other;
     
    int s, recv_len;
    unsigned int slen = sizeof(si_other);
    char buf[BUFLEN];
     
    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        die("socket");
    }
     
    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));
     
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
     
    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1) {
        die("bind");
    }
     
    // keep listening for data
    // no need to fork into another process - there is no connection
    // just answer and keep living
    while(1) {
        printf("Waiting for data...");
        fflush(stdout);
         
        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1) {
            die("recvfrom()");
        }
         
        //print details of the client/peer and the data received
        printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Data: %s\n" , buf);

        char answer[BUFLEN];
        checkReceivedMessage(buf, answer, NULL);
        printf("Answering...\n");
         
        //now reply the client with the same data
        if (sendto(s, answer, BUFLEN, 0, (struct sockaddr*) &si_other, slen) == -1) {
            die("sendto()");
        }
        printf("Done!\n\n");
    }
 
    close(s);
    return 0;
}

// AUXILIAR METHODS
/// Check an incomming request
/// to get the proper answer from the Data.c
/// PARAM:  message - the incoming request
///         answer  - the answer string
/// Return: true    - the correct answer could be found
///         false   - the answer was not found
bool checkReceivedMessage(char *message, char *answer, int* usr_type){

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
            if(*usr_type == 1) {
                strcpy(answer, "Next class ");
                strcat(answer, code);
                strcat(answer, ": ");
                getCommentFromRequest(message, comment);
                strcat(answer, comment);
                if (setComentarioForCode(code, comment) == false){
                    strcpy(answer, "The comment couldnt be written. Check the subject code\n");
                }
            } else {
                strcpy(answer, "Only teachers can set comments.\n");
            }
            break;
        case 9:
            strcpy(answer, "You are now logged as a teacher!\n");
            *usr_type = 1;
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

/// Default error method
/// print error message and kill the program
void die(char *s) {
    perror(s);
    exit(1);
}
