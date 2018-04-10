// Libraries and Reference Codes

/*  Giovani Nascimento Pereira - 168609
  Ignacio Espinoso Ribeiro - 

  MC833 - 2S2018
  Unicamp
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>
#include <signal.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>

#define MAXDATASIZE 2000 // max number of bytes we can get at once 

// Simplifying struct types
typedef struct addrinfo addrinfo;

typedef struct connectionTime{
	int operation;
	struct timeval sendTime;
	struct timeval recieveTime;
} connectionTime;

/// Print the connection time from a connectionTime struct variable
void printConnectionTimeClient(connectionTime op){
	printf(">>>>>Send Time: %d μs\n", op.sendTime.tv_usec);
	printf("<<<<<Recieve Time: %d μs\n", op.recieveTime.tv_usec);
	printf("Total Interval Time: %d μs\n", (op.recieveTime.tv_usec - op.sendTime.tv_usec));
}

/// Print the connection time from a connectionTime struct variable
void printExecutionTimeServer(connectionTime op){
	printf(">>>>>Recieve Time: %d μs\n", op.recieveTime.tv_usec);
	printf("<<<<<Send Time: %d μs\n", op.sendTime.tv_usec);
	printf("Total Processing Time: %d μs\n", (op.sendTime.tv_usec - op.recieveTime.tv_usec));
}