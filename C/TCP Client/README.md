#HTTP Client

###Author: Ron Amar

Submitted Files:
client.c - contains the code for the HTTP client

###==================== Remarks ====================

The client uses the HTTP/1.0 protocol.
The client can handle URLs with and without a port number. If no port number is given, the default port (80) is used.
The client can handle GET and POST HTTP requests, using the -r and -p flags, respectively. If no flag is given, the default request is GET. 
The client will return an error message if the URL is invalid or if the port number is out of the valid range (0-65535=2^16).
Additional feature: The client can handle URLs with arguments, such as "http://www.example.com/index.php -r 2 arg1=value1 arg2=value2". The arguments will be appended to the path in the HTTP request.

Pay Attention: in case the port given is not correct/not the default the connect() function will get stuck in an infinite loop (sometimes exit through connection error).

###==================== About the Code ====================
The code for the HTTP client is written in C and uses the standard libraries stdio.h, stdlib.h, string.h, netinet/in.h, netdb.h, and unistd.h. It includes the following main functions:

getType: checks for the type of argument (URL, GET or POST) and returns 'u' for URL, 'p' for -p, 'r' for -r, or 0 for none of the above.
handleURL: parses the URL and extracts the host, path, and port number. It returns the port number or -1 if the URL is invalid.
handleP: constructs a POST request using the content provided and sends it to the server. It returns 0 on success or -1 if the request failed.
handleR: constructs a GET request using the arguments provided and sends it to the server. It returns 0 on success or -1 if the request failed.

The code also includes helper functions for parsing arguments, checking if a string is a number, and appending arguments to the URL.

Overall, the code is designed to be simple and efficient, allowing the user to easily send HTTP requests to a server and receive the response.

###==================== Usage ====================
To use the client, compile the code with a C compiler and run the executable with the following arguments:
./client [-p n <text>] [-r n < pr1=value1 pr2=value2 ...>] <URL>

URL: the URL of the website to send the request to. This argument is required.
-p: flag to specify a POST request. If this flag is not provided, a GET request will be sent.
-r: flag to specify additional arguments to be appended to the URL. This flag can be used multiple times to add multiple arguments.

Examples:
To send a GET request to "http://www.example.com/index.html" using the default port (80):
./client "http://www.example.com/index.html"

To send a GET request to "http://www.example.com:8080/index.php" with the argument "arg2=value2" through port 8080:
./client "http://www.example.com:8080/index.php" -r 1 "arg2=value2"

To send a POST request to "http://www.example.com/index.php" with the content "content":
./client "http://www.example.com/index.php" -p <content-length> content

