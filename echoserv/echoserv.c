/*

  ECHOSERV.C
  ==========
  (c) Paul Griffiths, 1999
  Email: mail@paulgriffiths.net
  
  Simple TCP/IP echo server.

*/


#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */

#include "helper.h"           /*  our own helper functions  */

#include <stdlib.h>
#include <stdio.h>


/*  Global constants  */

#define ECHO_PORT          (2002)
#define MAX_LINE           (1000)


/* Function Decleration */

void handleConnection(int conn_s);


int main(int argc, char *argv[]) {
    int       list_s;                /*  listening socket          */
    int       conn_s;                /*  connection socket         */
    short int port;                  /*  port number               */
    struct    sockaddr_in servaddr;  /*  socket address structure  */
    char      buffer[MAX_LINE];      /*  character buffer          */
    char     *endptr;                /*  for strtol()              */


    /*  Get port number from the command line, and
        set to default port if no arguments were supplied  */

    if ( argc == 2 ) {
	port = strtol(argv[1], &endptr, 0);
	if ( *endptr ) {
	    fprintf(stderr, "ECHOSERV: Invalid port number.\n");
	    exit(EXIT_FAILURE);
	}
    }
    else if ( argc < 2 ) {
	port = ECHO_PORT;
    }
    else {
	fprintf(stderr, "ECHOSERV: Invalid arguments.\n");
	exit(EXIT_FAILURE);
    }

	
    /*  Create the listening socket  */

    if ( (list_s = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
	fprintf(stderr, "ECHOSERV: Error creating listening socket.\n");
	exit(EXIT_FAILURE);
    }


    /*  Set all bytes in socket address structure to
        zero, and fill in the relevant data members   */

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(port);


    /*  Bind our socket addresss to the 
	listening socket, and call listen()  */

    if ( bind(list_s, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
	fprintf(stderr, "ECHOSERV: Error calling bind()\n");
	exit(EXIT_FAILURE);
    }

    if ( listen(list_s, LISTENQ) < 0 ) {
	fprintf(stderr, "ECHOSERV: Error calling listen()\n");
	exit(EXIT_FAILURE);
    }

    
    /*  Enter an infinite loop to respond
        to client requests and echo input  */

    while ( 1 ) {

	/*  Wait for a connection, then accept() it  */

	if ( (conn_s = accept(list_s, NULL, NULL) ) < 0 ) {
        //printf("Connection accepted");
	    fprintf(stderr, "ECHOSERV: Error calling accept()\n");
	    exit(EXIT_FAILURE);
	}

    //printf("Anish%s\n");

	/*  Retrieve an input line from the connected socket
	    then simply write it back to the same socket.     */

	// Readline(conn_s, buffer, MAX_LINE-1);


    handleConnection(conn_s);
	// Writeline(conn_s, buffer, strlen(buffer));


	/*  Close the connected socket  */

	if ( close(conn_s) < 0 ) {
	    fprintf(stderr, "ECHOSERV: Error calling close()\n");
	    exit(EXIT_FAILURE);
	}

    //exit(0);
    }
}


void handleConnection(int conn_s) {

    int count;
    int type_count;
    int file_name_count;
    int buffer[MAX_LINE];
    int i;

    int nex;

    read(conn_s, &buffer, sizeof(int)*MAX_LINE);
    count = buffer[0];

    nex = 1;

    type_count = buffer[nex];
    nex += 1;

    char type[type_count+1];
    for (i=0; i<type_count; i++) {
        type[i] = (char) buffer[i+nex];
    }
    type[type_count+1] = '\0';
    nex += type_count;


    file_name_count = buffer[nex];
    nex += 1;

    char file_name[file_name_count+1];
    for (i=0; i<file_name_count; i++) {
        file_name[i] = (char) buffer[i+nex];
    }
    file_name[file_name_count+1] = '\0';
    nex += file_name_count;


    printf("%s\n", type);
    printf("%s\n", file_name);

    for (i=nex; i<count; i++) {
        printf("%d\n", buffer[i]);
    }
}



void readFile(int conn_s) {
    
}

void error(char *msg)                           // Error msg function that prints errors
{
    perror(msg);
    exit(1);
}

