Name: Anish Adhikari
ID: @02778744

Programming Project 1 
https://github.com/anish531213/TCP-Project-Netowrk

1.	Application Protocol

I have created my protocol for communication between Client and server after the socket connection is established.

There would be an error if the command line arguments for running client and server is incorrect.

Client
Server
Socket Connection established.
Socket Connection established
Reads data from provided filename, creates header and writes in socket.

Throws error if file length exceeds 1MB.
Creates header of about 20 bytes.

Message Type: Single integer buffer less than 4MB.

Format: integer array

Fields:  Total count, type count, type, file name size, file name, data count, data

Total count: Total number of integers in array counting header and data from file

Type count: Single integer with counting the length of type sent by client.

Type: Type (0/1/2/3) received from command line as an asci value in integer.
0    48, 1  49, 3  51

File name size: Single integer for size of filename.

File name: characters represented as asci integers. Eg.
target = 116->t, 97->a, 114->r, 103->g, 101->e, 116->t

Data count = Single integer for count of total data read from file

Data: Data stream of integers read from file.

Waits until any message is received from client.

If receives a message from client.

Reads the first 4 bytes which is total count integer.

Reads the remaining (count) integer from the socket and puts into respective fields.

Prints the buffer in format as below.

Amount Number1, Number2

Eg,
2 258,772
2 59,347
1 900
3 2330,48350,1
1 0
2 65535,0

Sends back error if there is format error in type or amount or in numbers.

Parses the type, file name and data and converts the type accordingly and saves into the folder where the server resides.
Reads four bytes from the socket to a integer named status.

If status == 0: prints “Format Error”
If status == 1: prints “Success”

Closes the connection and exits.

Sends status code as either 0/1 where 0 means failure and 1 means success.

Message Type: 4 bytes Integer with value 0 or 1.

If there is any format error while processing the buffer from the socket, writes ‘0’ integer immediately to the socket, closes and deletes the file.

If everything goes fine, at the end after closing the file pointer, writes ‘1’ integer to the socket.

Closes the connection.




2.	Test Cases
Description
Rationale
Content/Error
File size > 1MB
Making sure my server reads a fixed amount from the socket. As my server reads only 4MB, I am requiring my client to send file less than 1MB.
Client: File length exceed! Undefined error: 0

Server: No response, just waits.

Type error ( Type other than 0, 1, 2 or 3 )
Have to handle type error in client as server is made only to process type 0/1/2/3.
Client: Type variable is not correct. Please use type 0, 1, 2 or 3: Undefined error: 0

Server: No response.
Port error/Host error
Error in host name or port.

Client: ECHOCLNT: Error calling connect()

Server: No response
File not found
If file provided in client is not found. 
Client: Unable to open file!: No such file or directory

Server: No response
If file name field in command line argument is empty
If file name not provided correctly in command line, there is a problem.
Client: Must provide file_name.: Undefined error: 0

Server: No response
If first byte in file in client is not 0 or 1.
If the first byte in the file is not found 0 or 1 by server,
Server send failure code.
Client: Format Error

Server: First byte format is incorrect!

If type is not correct anywhere in the file 
If the file has inconsistency in type and amount, there is format error.
Client: Format Error

Server: Type 1 format error!
If the number in the client file exceed 65545.
If the number is greater than minimum value, there is format error.
Client: Format Error

Server: Invalid number for type 1
If everything goes out fine. 
If no format error or command line errors. File is saved successfully and server sends success code.
Client: Success

Server: 
2 258,772
2 59,347
1 900
3 2330,48350,1
1 0
2 65535,0


3.	Usage

The server program runs continuously until exited manually.

The client program runs and receives error or success and closes.


4.	Instruction to Compile
There are two folders echoserv/ and echoclnt/ in the main project folder.

Go to echoserv/ folder,
Compile the server code using command:
	gcc –o server echoserv.c

Run the server using command:
	./server <port>
eg.	./server 2345


Go to echoclnt/ folder,
Compile the client code using command:
	gcc –o client echoclnt.c

Run the client using command:
	./client <server IP> <server port> <file path> <to format> <to name>
eg. 	./client 127.0.0.1 2345 ../practice_project_test_file_1.dms target

5.	Problems
There were few cases where I experienced server code print negative numbers. 

6.	References
http://www.paulgriffiths.net/program/c/echoserv.php
http://www.paulgriffiths.net/program/c/echoclnt.php
https://stackoverflow.com




Github Repo Link 
https://github.com/anish531213/TCP-Project-Netowrk
