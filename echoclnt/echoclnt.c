/*

  ECHOCLNT.C
  ==========
  (c) Paul Griffiths, 1999
  Email: mail@paulgriffiths.net
  
  Simple TCP/IP echo client.

*/


#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */

#include "helper.h"           /*  Our own helper functions  */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/*  Global constants  */

#define MAX_LINE           (1000)


/*  Function declarations  */

int ParseCmdLine(int argc, char *argv[], char **szAddress, char **szPort, char **read_file, char** type, char** server_file);
//void handleConnections(int conn_s);
void handleConnections(int conn_s, char* type, char* read_file, char* server_file);
//char* Readfile(char* filename);
void error(char *msg); 


/*  main()  */

int main(int argc, char *argv[]) {

    int       conn_s;                /*  connection socket         */
    short int port;                  /*  port number               */
    struct    sockaddr_in servaddr;  /*  socket address structure  */
    char      buffer[MAX_LINE];      /*  character buffer          */
    char     *szAddress;             /*  Holds remote IP address   */
    char     *szPort;                /*  Holds remote port         */
    char     *endptr;                /*  for strtol()              */



    char     *type;                  /*  Holds type variable       */
    char     *read_file;             /*  file to read by client    */
    char     *server_file;           /*  file name to be saved at server  */



    /*  Get command line arguments  */

    ParseCmdLine(argc, argv, &szAddress, &szPort, &read_file, &type, &server_file);


    /*  Error if type is not ok  */

    if (strstr("0123", type) == NULL) {
        error("Type variable is not correct. Please use type 0, 1, 2 or 3");
    }

    /*  Set the remote port  */

    port = strtol(szPort, &endptr, 0);
    if ( *endptr ) {
	printf("ECHOCLNT: Invalid port supplied.\n");
	exit(EXIT_FAILURE);
    }
	

    /*  Create the listening socket  */

    if ( (conn_s = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
	fprintf(stderr, "ECHOCLNT: Error creating listening socket.\n");
	exit(EXIT_FAILURE);
    }


    /*  Set all bytes in socket address structure to
        zero, and fill in the relevant data members   */

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_port        = htons(port);


    /*  Set the remote IP address  */

    if ( inet_aton(szAddress, &servaddr.sin_addr) <= 0 ) {
	printf("ECHOCLNT: Invalid remote IP address.\n");
	exit(EXIT_FAILURE);
    }

    
    /*  connect() to the remote echo server  */

    if ( connect(conn_s, (struct sockaddr *) &servaddr, sizeof(servaddr) ) < 0 ) {
	printf("ECHOCLNT: Error calling connect()\n");
	exit(EXIT_FAILURE);
    }

    /*  Get string to echo from user  */

    // printf("Enter the string to echo: ");
    // fgets(buffer, MAX_LINE, stdin);
    

    /*  Send string to echo server, and retrieve response  */



    //Writeline(conn_s, buffer, strlen(buffer));
    //Readline(conn_s, buffer, MAX_LINE-1);


    handleConnections(conn_s, type, read_file, server_file);

    //printf("Echo response: %s\n", buffer);

    return EXIT_SUCCESS;
}


void handleConnections(int conn_s, char* type, char* read_file, char* server_file) {

    unsigned char buffer[1];

    int data_array[MAX_LINE];
    int count = 1;
    int i;
    //int header_count = 0;


    int type_size = strlen(type);
    
    int server_file_size = strlen(server_file);
    
    // copying type_size and server_file_size in dataarray
    
    data_array[count] = type_size;
    count++;
    for (i=0; i<type_size; i++) {
        data_array[count] = type[i];
        count++;
    }
       

    data_array[count] = server_file_size;
    count++;
    for (i=0; i<server_file_size; i++) {
        data_array[count] = server_file[i];
        count++;
    }
        


    FILE *ptr;

    ptr=fopen(read_file,"rb");

    if (!ptr) { 
        error("Unable to open file!");     
    }

    while (fread(&buffer,sizeof(buffer),1,ptr) != 0) {
        // Saving the file buffer into data array
        data_array[count] = buffer[0];
        count += 1;
        //printf("%u ", buffer[0]);
    }

        // printf("%d, ", count);

        // for(int i=0; i<count; i++) {
        //     printf("%d, ", data_array[i]);
        // }

    data_array[0] = count;

    printf("Writing to socket\n");

    printf("%d, ", count);

    write(conn_s, &data_array, sizeof(int)*count);


    // for(int i=0; i<count; i++) {
        
    //     //printf("%d, ", data_array[i]);

    //     write(conn_s, data_array[i], sizeof(int));
    // }

    printf("Successfully written to socket\n");


    fclose(ptr);

}



int ParseCmdLine(int argc, char *argv[], char **szAddress, char **szPort, char **read_file, char** type, char** server_file) {

    int n = 1;


    // Setting address and port from command line argument
    *szAddress = argv[1];
    *szPort = argv[2];
    *read_file = argv[3];
    *type = argv[4];
    *server_file = argv[5];

 //    while ( n < argc ) {
	// if ( !strncmp(argv[n], "-a", 2) || !strncmp(argv[n], "-A", 2) ) {
	//     *szAddress = argv[++n];
	// }
	// else if ( !strncmp(argv[n], "-p", 2) || !strncmp(argv[n], "-P", 2) ) {
	//     *szPort = argv[++n];
	// }
	// else if ( !strncmp(argv[n], "-h", 2) || !strncmp(argv[n], "-H", 2) ) {
	//     printf("Usage:\n\n");
	//     printf("    timeclnt -a (remote IP) -p (remote port)\n\n");
	//     exit(EXIT_SUCCESS);
	// }
	// ++n;
 //    }

    return 0;
}

void error(char *msg)                           // Error msg function that prints errors
{
    perror(msg);
    exit(1);
}

