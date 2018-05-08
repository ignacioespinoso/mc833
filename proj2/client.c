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

// Global variable for time manangement
connectionTime op;
 
int main(int argc, char *argv[]) {
    struct sockaddr_in si_other;
    int s;
    unsigned int slen=sizeof(si_other);
    char buf[BUFLEN];
    char message[BUFLEN];

    // Get server IP address
    checkInformation(argc);
    char *server = argv[1];
 
 	// Create UDP socket
    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        die("socket");
    }
 
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
    
    if (inet_aton(server , &si_other.sin_addr) == 0) {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
 
    while(1) {

        // Check if the user tried to disconnect
        // or get the next servre request
        if (selectRequestMessage(message) == false) {
        	break;
        }
         
        //Record send Time
    	gettimeofday(&op.sendTime, NULL);
    	//Send the message
        if (sendto(s, message, strlen(message), 0, (struct sockaddr *) &si_other, slen)==-1) {
            die("sendto()");
        }
         
        //receive a reply and print it
        //clear the buffer by filling null, it might have previously received data
        memset(buf,'\0', BUFLEN);
        //try to receive some data, this is a blocking call
        if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1) {
            die("recvfrom()");
        }
        // Get the received time
    	gettimeofday(&op.receiveTime, NULL);

        puts(buf);

        // Print the time
    	printConnectionTimeClient(op);

        //Make a little stop
        printf("\n(Press Enter to continue)\n\n");
        getchar();
        getchar();
    }
 
    close(s);
    return 0;
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
