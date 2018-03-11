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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/*  Global constants  */

#define MAX_LINE           (1000)
#define HEADER_LINE        (100)


/*  Function declarations  */

int ParseCmdLine(int argc, char *argv[], char **szAddress, char **szPort, char **read_file, char** type, char** server_file);
void handleConnections(int conn_s, char* type, char* read_file, char* server_file);
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
    int      status;                 /*  Holds status of process   */



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

 
    /*  Handling writing file to socket  */

    handleConnections(conn_s, type, read_file, server_file);

    /*  Reading integer status from the socket  */

    read(conn_s, &status, sizeof(int));

    /*  Printing message according to server status  */

    if (status == 0) {
        printf("Format Error\n");
    } else {
        printf("Success\n");
    }

    return EXIT_SUCCESS;
}


/*  Handles reading data from file, creating header and sending data to server  */

void handleConnections(int conn_s, char* type, char* read_file, char* server_file) {

    unsigned char buffer[1];                        // reading buffer initialized

    int data_array[MAX_LINE+HEADER_LINE];           // buffer to store data and header
    int count = 1;                                  // intial buffer position
    int i;                                          // looping variable

    int type_size = strlen(type);                   // getting length of type
    
    int server_file_size = strlen(server_file);     // getting length of server file name
    

    /*  Copying type_size and type in dataarray  */
    
    data_array[count] = type_size;
    count++;                                        // incrementing buffer postion
    for (i=0; i<type_size; i++) {
        data_array[count] = type[i];
        count++;
    }

    /*  Copying file_name size and file_name in dataarray  */

    data_array[count] = server_file_size;
    count++;                                         // incrementing buffer postion
    for (i=0; i<server_file_size; i++) {
        data_array[count] = server_file[i];
        count++;
    }
        
    /*  Reading file given by command line in binary  */

    FILE *ptr;
    ptr=fopen(read_file,"rb");                    

    /*  If error in reading file  */

    if (!ptr) { 
        error("Unable to open file!");     
    }

    /*  Checking the file length if it doesn't exceed 1000 bytes  */

    fseek(ptr, 0, SEEK_END);
    int lengthOfFile = ftell(ptr);
    rewind(ptr);


    /*  Throwing error if filelength exceed 1000 bytes  */
    
    if (lengthOfFile > 1000) {
        error("File length exceed!");
    }

   /*  Reading the file 1 byte at a time and saving into data_array as integer  */

    while (fread(&buffer,sizeof(buffer),1,ptr) != 0) {
        /*  Saving the file buffer into data array  */
        data_array[count] = buffer[0];
        count += 1;
    }

    /*  Providing total count at beginning of buffer  */

    data_array[0] = count;                      

    /*  Writing the header and file data in integer to the socket  */

    write(conn_s, &data_array, sizeof(int)*count);  


    fclose(ptr);                                    // closing the file

}



int ParseCmdLine(int argc, char *argv[], char **szAddress, char **szPort, char **read_file, char** type, char** server_file) {

    /* If first argument not present */

    if (argv[1] == NULL) {
        error("Must provide server address");
    }

    /* If second argument not present */

    if (argv[2] == NULL) {
        error("Must provide port number");
    }

    /* If third argument not present */

    if (argv[3] == NULL) {
        error("Must provide reading file name");
    }

    /* If fourth argument not present */

    if (argv[4] == NULL) {
        error("Must provide host name");
    }

    /* If fifth argument not present */
  
    if (argv[5]==NULL) {
        error("Must provide file_name.");
    }

    /* 
        Setting address, port, read_file, type and server_file 
        from command line argument
    */

    *szAddress = argv[1];
    *szPort = argv[2];
    *read_file = argv[3];
    *type = argv[4];
    *server_file = argv[5];

    return 0;
}

/*  Error msg function that prints errors  */

void error(char *msg)                         
{
    perror(msg);
    exit(1);
}

