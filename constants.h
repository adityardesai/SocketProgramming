
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>


#define PORTCLTCP "25115"

#define UDPPORTA "21115"
#define UDPPORTB "22115"
#define UDPPORTC "23115"
#define UDPPORTD "24115"

#define BACKLOG 10 // how many pending connections queue will hold
#define MAXDATASIZE 1024 // max number of bytes we can get at once
#define MAX 1024

struct network {
	char hostname[MAX];
	char peername[MAX];
	int distance;
	struct node *next;
};
