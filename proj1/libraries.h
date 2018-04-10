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
#include "timeManager.c"

#define MAXDATASIZE 2000 // max number of bytes we can get at once 

// Simplifying struct types
typedef struct addrinfo addrinfo;
