// Server Code
// Reference:   https://www.thegeekstuff.com/2011/12/c-socket-programming/
//              https://www.thegeekstuff.com/2011/12/c-socket-programming/
/*  Giovani Nascimento Pereira - 168609
    Ignacio Espinoso Ribeiro - 

    MC833 - 2S2018
    Unicamp
*/
#include "libraries.h"
#include "Data.c"

#define MYPORT "3490"  // the port users will be connecting to
#define BACKLOG 5     // how many pending connections queue will hold

int main(int argc, char *argv[]) {

    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 

    char sendBuff[1025];
    time_t ticks; 

    // Creates an UN-named socket inside the kernel and 
    // returns an integer known as socket descriptor
    printf("CREATING SOCKET...\n");
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000); 

    printf("BINDING...\n");
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    printf("LISTENING...\n\n");
    listen(listenfd, 10); 

    while(1) {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 

        printf("Accepted %d\n", listenfd);

        ticks = time(NULL);
        snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n%s\r\n", ctime(&ticks), getEmentaFromCodigo("MC102"));
        
        printf("--->>SENDING: %s\n ", sendBuff);
        write(connfd, sendBuff, strlen(sendBuff)); 

        close(connfd);
        sleep(1);
     }
}
