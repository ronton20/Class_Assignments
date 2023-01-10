Author: Ron Amar
ID:
Exercise: TCP Server

Submitted Files:
server.c - contains the code for the TCP server
threadpool.c - contains the code for the threadpool used by the server to handle client requests

==================== Remarks ====================

The server uses the TCP protocol to handle client requests.
The server uses a threadpool to handle all client requests, ensuring efficient and concurrent handling of multiple requests.
The server can handle requests for files and directories, returning the appropriate response to the client.
The server checks for permission of the requested file/directory before returning it to the client. If permission is denied, an error message is returned to the client.

There is a problem returning mp3 files in *CHROME browser* specifically, for some reason chrome sends 2 requests at a time,
where one of the requests are closed immediatly thus resulting in a SIGPIPE error during the write (system call) function. 

==================== About the Code ====================
The code for the TCP server is written in C and uses the standard libraries stdio.h, stdlib.h, string.h, netinet/in.h, and unistd.h. It includes the following main functions:

returnError: returns an error message to the client based on the provided error number.
checkPathPermissions: checks for the permissions of the requested file/directory path and returns 0 if permission is granted or -1 if permission is denied.
respondFile: responds to the client with the requested file if permission is granted. Returns an error message if permission is denied.
respondDirectory: responds to the client with the requested directory if permission is granted. Returns an error message if permission is denied.
handle_request: handles a request from the client, checks for the request type (file or directory) and calls the appropriate response function.

The threadpool code in threadpool.c includes functions for creating and managing the threadpool, as well as a function for handling client requests in parallel using the threadpool.

Overall, the code is designed to be secure and efficient, allowing the server to handle multiple requests concurrently and ensure proper permissions for requested files and directories.

==================== Usage ====================
To use the server, compile the code with a C compiler and run the executable with the following arguments:
./server <port-number> <pool-size> <max-number-of-request>

port-number: the port number to run the server on. This argument is required.
pool-size: the size of the threadpool. This argument is required.
max-number-of-request: the maximum number of requests the server will recieve before it shuts down. This argument is required.

Once the server is running, clients can connect and make requests for files and directories.
The server will respond with the requested file/directory or an error message if permission is denied or the request is invalid