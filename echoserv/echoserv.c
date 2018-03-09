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

int handleConnection(int conn_s);


int main(int argc, char *argv[]) {
    int       list_s;                /*  listening socket          */
    int       conn_s;                /*  connection socket         */
    short int port;                  /*  port number               */
    struct    sockaddr_in servaddr;  /*  socket address structure  */
    char      buffer[MAX_LINE];      /*  character buffer          */
    char     *endptr;                /*  for strtol()              */

    int       status;                /*  Status code 0 or 1        */


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


    /*  Connection Haldler for reading from client  */

    status = handleConnection(conn_s);
	// Writeline(conn_s, buffer, strlen(buffer));


	/*  Close the connected socket  */

	if ( close(conn_s) < 0 ) {
	    fprintf(stderr, "ECHOSERV: Error calling close()\n");
	    exit(EXIT_FAILURE);
	}

    //exit(0);
    }
}


int handleConnection(int conn_s) {

    int count;                          /*  Toatal size of buffer     */
    int type_count;                     /*  Size for type variable    */
    int file_name_count;                /*  Length of file name       */
    int buffer[MAX_LINE];               /*  Buffer for reading fron socket   */
    int i;                              /*  Looping variable i        */

    int nex;                            /*  Counter for Buffer        */
    int error_status;                   /*  Error status              */


    /*  Reading fron the socket into buffer   */
    read(conn_s, &buffer, sizeof(int)*MAX_LINE); 

    /*  Length of buffer  */
    count = buffer[0];

    nex = 1;                            // Starting buffer location

    type_count = buffer[nex];           // Getting size of types
    nex += 1;                           // Incrementing buffer location

    /*  Reading type from buffer  */
    char type[type_count+1];            
    for (i=0; i<type_count; i++) {      
        type[i] = (char) buffer[i+nex];
    }
    type[type_count+1] = '\0';          // Terminating with null to make string
    nex += type_count;                  // Incrementing buffer location


    file_name_count = buffer[nex];      // Getting size of filename
    nex += 1;                           // Incrementing buffer location

    /*  Reading filename from buffer  */
    char file_name[file_name_count+1];
    for (i=0; i<file_name_count; i++) {
        file_name[i] = (char) buffer[i+nex];
    }
    file_name[file_name_count+1] = '\0';// Terminating with null to make string
    nex += file_name_count;             // Incrementing buffer location


    buffer[count] = '\0';               // Terminating the whole buffer for conversion

    /*  Sending file to convert  */
    
    error_status = convert(file_name, type, buffer, count, nex);

    return error_status;
}

/*  Error msg function that prints errors  */

void error(char *msg)               
{
    perror(msg);
    exit(1);
}

