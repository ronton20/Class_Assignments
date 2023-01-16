# TCP Chat Server

Author: Ron Amar

## Submitted Files
- chatServer.h
- chatServer.c

## Remarks
This is a basic TCP server implementation in C.
It uses the `select()` function to handle multiple client connections and uses a connection pool to keep track of the active connections.
The server listens for incoming connections on the specified port, and it uses a signal handler to gracefully exit when the user sends an interrupt signal (`Ctrl+C`).

## About the Code
The code includes two files: "chatServer.h" and "chatServer.c".
`chatServer.h` contains the declarations for the data structures and functions used in the server.
`chatServer.c` contains the implementation of the server, including the main function and signal handler.
The server uses a connection pool to keep track of the active connections, it uses a doubly-linked list to handle the messages that have to be written out to the clients.

## Usage
To run the server, use the following command:
```C
./server <port>
```
Where `<port>` is the port number on which the server should listen for incoming connections. The port number must be between 1 and 2^16.

For example, to start the server on port 8000, use the following command:
```C
./server 8000
```
You can stop the server by sending an interrupt signal (`Ctrl+C`) to the terminal.
