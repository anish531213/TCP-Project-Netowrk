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

int ParseCmdLine(int argc, char *argv[], char **szAddress, char **szPort);
//void handleConnections(int conn_s);
void handleConnections(int conn_s);
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


    /*  Get command line arguments  */

    ParseCmdLine(argc, argv, &szAddress, &szPort);


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


    handleConnections(conn_s);

    /*

    FILE *fptr;

    printf("Reading for the file\n");

    if ((fptr = fopen("../Output.dms", "rb")) == NULL) {
        error("File not found..");
    } 

    else {
        char *fileContent = Readfile("../Output.dms");

        if (fileContent) {
            char send[strlen(fileContent)];
            fileContent[strlen(fileContent) - 1] = '\0';
            int file_size = strlen(fileContent);
            sprintf(send, "%d", file_size);
            strcat(send, "\n");
            strcat(send, fileContent);

            printf("%s\n", send);

            write(conn_s, send, strlen(send));
        }
    }


    printf("File sent to server");

    */

    /*  Output echoed string  */

    //printf("Echo response: %s\n", buffer);

    return EXIT_SUCCESS;
}


void handleConnections(int conn_s) {

    unsigned char buffer[1];

    int data_array[MAX_LINE];
    int count = 1;

    FILE *ptr;

    ptr=fopen("../practice_project_test_file_1.dms","rb");

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

}


/*

char* Readfile(char* filename) {
    char* buffer = NULL;
    int filesize, readsize;

    FILE *fp = fopen(filename, "rb");

    if (fp) {
        fseek(fp, 0, SEEK_END);        // pointing to the end of the file to calculate total byte size
        filesize = ftell(fp);
        rewind(fp);                     // Restoring the pointer to original location


        // Allocating buffer string for file
        buffer = (char*) malloc(sizeof(char) * (filesize + 1) );

        readsize = fread(buffer, sizeof(char), filesize, fp);

        if (filesize != readsize) {
            // Something went wrong
            printf("Something is wrong while reading.\n");
            free(buffer);
            buffer = NULL;
        }

        fclose(fp);
    }
 
    return buffer;
}


void handleConnections(int conn_s) {
    int n;
    char file_buffer[MAX_LINE];
    char new_buff[MAX_LINE-1];
    char send_buff[MAX_LINE-1];
    int size;
    char fsize[2];
    FILE *fptr;                             // Declaring a file pointer


    //strcpy(file_buffer, "../Output.txt");

    //if ( access(file_buffer, F_OK) != -1) {   // Checks if the file is present 

    if( (fptr = fopen("../Output.txt", "rb")) > 0) {      // Opens the file for binary read

        fseek(fptr, 0, SEEK_END);        // pointing to the end of the file to calculate total byte size
        size = ftell(fptr);
        rewind(fptr);                     // Restoring the pointer to original location

        sprintf(send_buff, "%d", size);     
        strcat(send_buff, "\n");

        // while(!feof(fptr))
        // {
        fread(new_buff, MAX_LINE, 1, fptr); // Reading the file and saving into new_buff
        strcat(send_buff, new_buff);        // Concatenating the file size and file data in send_buff
        // }
        

    } else {
        exit(EXIT_FAILURE);                   // Sending not found on file not found case
    }

    n = write(conn_s, send_buff, strlen(send_buff));    // Writing the file data buffer into the socket

    if (n < 0) error("ERROR writing to socket");

}

*/


int ParseCmdLine(int argc, char *argv[], char **szAddress, char **szPort) {

    int n = 1;

    while ( n < argc ) {
	if ( !strncmp(argv[n], "-a", 2) || !strncmp(argv[n], "-A", 2) ) {
	    *szAddress = argv[++n];
	}
	else if ( !strncmp(argv[n], "-p", 2) || !strncmp(argv[n], "-P", 2) ) {
	    *szPort = argv[++n];
	}
	else if ( !strncmp(argv[n], "-h", 2) || !strncmp(argv[n], "-H", 2) ) {
	    printf("Usage:\n\n");
	    printf("    timeclnt -a (remote IP) -p (remote port)\n\n");
	    exit(EXIT_SUCCESS);
	}
	++n;
    }

    return 0;
}

void error(char *msg)                           // Error msg function that prints errors
{
    perror(msg);
    exit(1);
}

