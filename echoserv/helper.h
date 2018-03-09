/*

  HELPER.H
  ========
  (c) Paul Griffiths, 1999
  Email: mail@paulgriffiths.net

  Interface to socket helper functions. 

  Many of these functions are adapted from, inspired by, or 
  otherwise shamelessly plagiarised from "Unix Network 
  Programming", W Richard Stevens (Prentice Hall).

*/


#ifndef PG_SOCK_HELP
#define PG_SOCK_HELP


#include <unistd.h>             /*  for ssize_t data type  */



#define LISTENQ        (1024)   /*  Backlog for listen()   */


/*  Function declarations  */

void convert(char* file_name, char* char_type, int* data_array, int count, int start);









#endif  /*  PG_SOCK_HELP  */

