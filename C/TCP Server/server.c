#include "threadpool.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <signal.h>
// #include <sys/sendfile.h>


#define RFC1123FMT "%a, %d %b %Y %H:%M:%S GMT"
#define LISTEN_Q_SIZE 5
#define BUF_SIZE 2048
#define TIME_BUF_SIZE 128
#define STR_SIZE 64

// Returns the current time in GMT format
    void getTime(char* timebuf, size_t size) {
    if(timebuf == NULL) return;
    time_t now = time(NULL);
    strftime(timebuf, size, RFC1123FMT, gmtime(&now));
}

// The usage message to display on usage error
void usageMessage() {
    printf("Usage: server <port> <pool-size> <max-number-of-request>\n");
    exit(EXIT_FAILURE);
}

//checks if a string is a number, returns -1 if not, 0 if yes
int isNumber(const char* str) {
  if(str == NULL || str[0] == 0) return -1;
  int i = 0;
  while(str[i] != 0) {
    if(!(str[i] >= '0' && str[i] <= '9')) return -1;
    i++;
  }
  return 0;
}

// Frees an array of strings
void freeargv(char** argv) {
    if(argv == NULL) return;
    int i = 0;
    while(argv[i] != NULL) {
        free(argv[i++]);
    }
    free(argv);
    argv = NULL;
}

// Splits a string to <num_of_tokens> tokens by <delimeter>
char** splitstr(const char* str, const char* delimeter, int num_of_tokens) {
    if(str == NULL || num_of_tokens <= 0) return NULL;
    // Copy str to a new string to split
    char copy[BUF_SIZE] = "";
    strcpy(copy, str);
    // Create argv
    char** argv = (char**)malloc((num_of_tokens + 1) * sizeof(char*));
    if(argv == NULL) {
        perror("malloc() failed");
        return NULL;
    }
    // Initialize argv to NULLs
    for(int i = 0; i < num_of_tokens + 1; i++) {
        argv[i] = NULL;
    }
    char* token = strtok(copy, delimeter);
    int i = 0;
    // Create a malloc'd token and insert it to argv
    while(token != NULL) {
        if(i >= num_of_tokens) {
            freeargv(argv);
            argv = NULL;
            return argv;
        }
        argv[i] = (char*)malloc((strlen(token) + 1) * sizeof(char));
        strcpy(argv[i], token);
        token = strtok(NULL, delimeter);
        i++;
    }
    // Check if there are more tokens then requested
    if(i < num_of_tokens) {
        freeargv(argv);
        argv = NULL;
    }

    return argv;
}

// Sends a response to the client
void respond(int fd, char* response, int response_length) {
    int bytes_sent = 0;
    int bytes_left = response_length;
    int n = 0;
    while(bytes_sent < response_length) {
        n = send(fd, response + bytes_sent, bytes_left, 0);
        if(n == -1) {
            perror("send() failed");
            break;
        }
        bytes_sent += n;
        bytes_left -= n;
    }
}

// Returns a string of an error based on the <err> number
void returnError(int fd, int err, char* path) {
    char error[STR_SIZE] = "";
    char location[STR_SIZE] = "";
    char error_body[STR_SIZE] = "";
    switch(err) {
        case 302:
            strcat(error, "302 Found");
            strcat(location, "Location: ");
            strcat(location, path);
            strcat(location, "/\r\n");
            strcat(error_body, "Directories must end with a slash.");
            break;
        case 400:
            strcat(error, "400 Bad Request");
            strcat(error_body, "Bad Request.");
            break;
        case 403:
            strcat(error, "403 Forbidden");
            strcat(error_body, "Access denied.");
            break;
        case 404:
            strcat(error, "404 Not Found");
            strcat(error_body, "File not found.");
            break;
        case 500:
            strcat(error, "500 Internal Server Error");
            strcat(error_body, "Some server side error.");
            break;
        case 501:
            strcat(error, "501 Not supported");
            strcat(error_body, "Method is not supported.");
            break;
        default:
            return;
    }
    // Building the body of the error
    char body[BUF_SIZE] = "";
    strcat(body, "<HTML><HEAD><TITLE>");
    strcat(body, error);
    strcat(body, "</TITLE></HEAD>\r\n");
    strcat(body, "<BODY><H4>");
    strcat(body, error);
    strcat(body, "</H4>\r\n");
    strcat(body, error_body);
    strcat(body, "\r\n</BODY></HTML>");

    // Building the header of the error
    char header[BUF_SIZE] = "";
    strcat(header, "HTTP/1.0 ");
    strcat(header, error);
    strcat(header, "\r\nServer: webserver/1.0\r\nDate: ");
    char time[TIME_BUF_SIZE] = "";
    getTime(time, sizeof(time));
    strcat(header, time);
    strcat(header, "\r\n");
    strcat(header, location);  // Empty if not 302
    strcat(header, "Content-Type: text/html\r\n");
    char body_len[STR_SIZE] = "";
    sprintf(body_len, "Content-Length: %ld\r\n", strlen(body));
    strcat(header, body_len);
    strcat(header, "Connection: close\r\n\r\n");

    // Building the entire error message
    char* res = (char*)malloc((strlen(header) + strlen(body) + 1) * sizeof(char));
    if(res == NULL) {
        perror("malloc() failed");
        return;
    }
    res[0] = 0;
    strcat(res, header);
    strcat(res, body);

    // Sending the error message
    respond(fd, res, strlen(res));
    free(res);
}

// Returns the mime type of a file
char *get_mime_type(char *name) {
    char *ext = strrchr(name, '.');
    if (!ext) return NULL;
    // if (strcmp(ext, ".txt") == 0) return "text/plain";
    if (strcmp(ext, ".html") == 0 || strcmp(ext, ".htm") == 0) return "text/html";
    if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) return "image/jpeg";
    if (strcmp(ext, ".gif") == 0) return "image/gif";
    if (strcmp(ext, ".png") == 0) return "image/png";
    if (strcmp(ext, ".css") == 0) return "text/css";
    if (strcmp(ext, ".au") == 0) return "audio/basic";
    if (strcmp(ext, ".wav") == 0) return "audio/wav";
    if (strcmp(ext, ".avi") == 0) return "video/x-msvideo";
    if (strcmp(ext, ".mpeg") == 0 || strcmp(ext, ".mpg") == 0) return "video/mpeg";
    if (strcmp(ext, ".mp3") == 0) return "audio/mpeg";
    return NULL;
}

// Check permissions of the entire path
int checkPathPermissions(char* path) {
    if(path == NULL) return -1;
    struct stat st;
    char *cur_path;
    char *next_path;
    int ret;

    cur_path = strdup(path);
    if (cur_path == NULL) {
        perror("strdup() failed");
        return -1;
    }
    next_path = cur_path;

    while (1) {
        // Get the next component of the path
        next_path = strchr(next_path, '/');
        if (next_path == NULL) {
            // We have reached the end of the path
            break;
        }
        // Null terminate the current component of the path
        *next_path = '\0';

        // Check the permissions of the current component
        ret = stat(cur_path, &st);
        if (ret != 0) {
            perror("stat() failed");
            free(cur_path);
            return -1;
        }
        if (!S_ISDIR(st.st_mode)) {
            // The current component is not a directory
            free(cur_path);
            return -1;
        }
        if ((st.st_mode & S_IXOTH) == 0) {
            // The current component is not executable by others
            free(cur_path);
            return -1;
        }
        // Restore the '/' and move to the next component
        *next_path = '/';
        next_path++;
    }

    // Check if the final component has read permissions
    ret = stat(path, &st);
    if (ret != 0) {
        perror("stat() failed");
        free(cur_path);
        return -1;
    }
    if ((st.st_mode & S_IROTH) == 0) {
        // The file is not readable by others
        free(cur_path);
        return -1;
    }
    // Free the memory allocated by strdup()
    free(cur_path);
    // The path is valid
    return 0;
}

// Responds to the client in case its a file
void respondFile(int fd, char* file_path) {
    struct stat path;
    if(stat(file_path, &path) == -1) {
        perror("stat() failed");
        returnError(fd, 404, NULL);
        return;
    }
    // Get current time
    char curr_time[TIME_BUF_SIZE] = "";
    getTime(curr_time, sizeof(curr_time));
    // Get mime type
    char* file_type = get_mime_type(file_path);
    // Get file last modified time
    char file_time[TIME_BUF_SIZE];
    if(strftime(file_time, sizeof(file_time), RFC1123FMT, localtime(&path.st_mtime)) < 0) {
        perror("strftime() failed");
        returnError(fd, 500, NULL); // Internal server error
        return;
    }
    // Create header
    char header[BUF_SIZE] = "";
    if(file_type == NULL) {
        sprintf(header, "HTTP/1.0 200 OK\r\n"
                        "Server: webserver/1.0\r\n"
                        "Date: %s\r\n"
                        "Content-Length: %ld\r\n"
                        "Last-Modified: %s\r\n"
                        "Connection: close\r\n\r\n",
                        curr_time, path.st_size, file_time);
    } else {
        sprintf(header, "HTTP/1.0 200 OK\r\n"
                        "Server: webserver/1.0\r\n"
                        "Date: %s\r\n"
                        "Content-Type: %s\r\n"
                        "Content-Length: %ld\r\n"
                        "Last-Modified: %s\r\n"
                        "Connection: close\r\n\r\n",
                        curr_time, file_type, path.st_size, file_time);
    }

    // Send header
    if(write(fd, header, strlen(header)) < 0) {
        perror("write() failed");
        returnError(fd, 500, NULL); // Internal server error
        return;
    }
    // printf("Thread %ld:\nSent header:\n%s\nTo client fd: %d\n\n", pthread_self(), header, fd);
    // Open file
    FILE* readFile = fopen(file_path, "r");
    if(readFile < 0) {
        perror("fopen() failed");
        returnError(fd, 500, NULL); // Internal server error
        return;
    }
    // Read file and send it to the client

    /* GENERAL WAY TO SEND FILE - WORKS WITH *MOST* BROWSERS */
    // Change fread buffer size to: sizeof(buf)
    unsigned char buf[BUF_SIZE] = "";

    /* FIX FOR CHROME SIGPIPE */
    // Change fread buffer size to: path.st_size
    // unsigned char* buf = (unsigned char*)malloc(sizeof(unsigned char) * path.st_size);
    
    // Send using sendFile()
    // sendfile(fd, fileno(readFile), NULL, path.st_size);

    int bytes_read;
    // printf("Thread %ld: Sending file %s\nFrom file fd: %d\nTo client fd: %d\n", pthread_self(), file_path, fileno(readFile), fd);
    while((bytes_read = fread(buf, 1, sizeof(buf), readFile)) > 0) {
        // printf("Thread %ld: Read %d bytes from file %s\n", pthread_self(), bytes_read, file_path);
        ssize_t bytes_written;
        
        if((bytes_written = write(fd, buf, bytes_read)) < 0) {
            perror("write() failed");
            fclose(readFile);
            // free(buf);
            return;
        }
        // printf("Thread %ld: Wrote %ld bytes to client\n", pthread_self(), bytes_written);
    }
    // printf("Thread %ld: Done sending file %s\nFrom file fd: %d\nTo client fd: %d\n", pthread_self(), file_path, fileno(readFile), fd);
    fclose(readFile);
    // free(buf);
}

// Responds to the client in case its a directory
void respondDirectory(int fd, char* dir_path) {
    char newPath[BUF_SIZE] = "";
    strcat(newPath, dir_path + 1);
    strcat(newPath, "index.html");
    struct stat path;
    // Check if index.html exists in this dir
    if(stat(newPath, &path) == 0) {
        respondFile(fd, newPath);
        return;
    } else {
        // Create response body
        char* body = (char*)malloc(BUF_SIZE * sizeof(char));
        body[0] = 0;
        sprintf(body,   "<HTML>\r\n"
                        "<HEAD><TITLE>Index of %s</TITLE></HEAD>\r\n"
                        "<BODY>\r\n"
                        "<H4>Index of %s</H4>\r\n"
                        "<table CELLSPACING=8>\r\n"
                        "<tr><th>Name</th><th>Last Modified</th><th>Size</th></tr>\r\n\r\n",
                        dir_path, dir_path);
        // Open directory
        DIR *d;
        struct dirent *dir;
        char dir_name[BUF_SIZE] = ".";  // current directory
        if (dir_path[1] != '\0') {
            strcat(dir_name, dir_path);
        }

        // Open directory
        d = opendir(dir_name);
        if (d) {
            // Read entries in directory
            while ((dir = readdir(d)) != NULL) {

                body = (char*)realloc(body, (strlen(body) + BUF_SIZE) * sizeof(char));

                // Get file stats
                char file_path[BUF_SIZE] = "";
                strcat(file_path, dir_name);
                strcat(file_path, "/");
                strcat(file_path, dir->d_name);
                struct stat path;
                if(stat(file_path, &path) == -1) {
                    perror("stat() failed");
                    free(body);
                    returnError(fd, 500, NULL); // Internal server error
                    return;
                }

                // Get file last modified time
                // char* time = ctime(&path.st_mtime);  <---- another way to get time
                char time[TIME_BUF_SIZE];
                if(strftime(time, sizeof(time), RFC1123FMT, localtime(&path.st_mtime)) < 0) {
                    perror("strftime() failed");
                    free(body);
                    returnError(fd, 500, NULL); // Internal server error
                    return;
                }

                // Get file size - only if it's a regular file
                char size[STR_SIZE] = "";
                if(S_ISREG(path.st_mode))
                    sprintf(size, "%ld", path.st_size);

                // Add entry to body
                strcat(body, "<tr>\r\n<td><A HREF=\"");
                strcat(body, dir->d_name);
                if(S_ISDIR(path.st_mode))
                    strcat(body, "/");
                strcat(body, "\">");
                strcat(body, dir->d_name);
                strcat(body, "</A></td><td>");
                strcat(body, time);
                strcat(body, "</td>\r\n<td>");
                strcat(body, size);
                strcat(body, "</td></td>\r\n\r\n");
                
            }
            // Finish body
            strcat(body, "</table>\r\n\r\n");
            strcat(body, "<HR>\r\n\r\n");
            strcat(body, "<ADDRESS>webserver/1.0</ADDRESS>\r\n\r\n");
            strcat(body, "</BODY></HTML>");

            char time[TIME_BUF_SIZE] = "";
            getTime(time, sizeof(time));

            // Create header
            char header[BUF_SIZE] = "";
            sprintf(header, "HTTP/1.0 200 OK\r\n"
                            "Server: webserver/1.0\r\n"
                            "Date: %s\r\n"
                            "Content-Type: text/html\r\n"
                            "Content-Length: %ld\r\n"
                            "Connection: close\r\n\r\n",
                            time, strlen(body));

            char* response = (char*)malloc((strlen(header) + strlen(body) + 1) * sizeof(char));
            response[0] = 0;
            strcat(response, header);
            strcat(response, body);

            // Send response
            respond(fd, response, strlen(response));

            free(body);
            free(response);
            // Close directory
            closedir(d);
        }
    }
}

// Handles a request (assuming argv is of size 3)
void handle_request(int fd, char** argv) {
    // Check if request is valid
    if(argv == NULL || argv[0] == NULL || argv[1] == NULL || argv[2] == NULL 
       || strlen(argv[0]) <= 0 || strlen(argv[1]) <= 0 || strlen(argv[2]) <= 0) {
        returnError(fd, 400, NULL); // Bad request
        return;
    }
    // Check if method is GET
    if(strcmp(argv[0], "GET") != 0) {
        returnError(fd, 501, NULL); // Not supported
        return;
    }
    // Check if path is valid
    if(argv[1][0] != '/') {
        returnError(fd, 400, NULL); // Bad request
        return;
    }
    // Check if HTTP version is valid
    if(!(strcmp(argv[2], "HTTP/1.0") == 0 || strcmp(argv[2], "HTTP/1.1") == 0)) {
        returnError(fd, 400, NULL); // Bad request
        return;
    }
    // Add '.' to path
    char newPath[BUF_SIZE] = ".";
    strcat(newPath, argv[1]);

    // Check if path exists
    struct stat path;
    if(stat(newPath, &path) == -1) {
        returnError(fd, 404, NULL); // Not found
        return;
    }
    // Check path permissions
    if(checkPathPermissions(newPath) == -1) {
        returnError(fd, 403, NULL);
        return;
    }
    if(S_ISDIR(path.st_mode)) {
        //Path is a directory
        if(newPath[strlen(newPath) - 1] != '/') {
            returnError(fd, 302, argv[1]); // Found
            return;
        }
        respondDirectory(fd, argv[1]);
        return;
    }
    //Path is a file
    respondFile(fd, newPath);
}

// Checks the request and handles it
void* check_request(void* _fd) {
    if(_fd == NULL) return NULL;
    int fd = *((int*)_fd);
    free(_fd);
    if(fd <= 0) return NULL;

    // Read header from client
    char buf[BUF_SIZE] = "";
    char request[BUF_SIZE] = "";
    int nbytes = 0;
    while(1) {
        if((nbytes = read(fd, buf, sizeof(buf) - sizeof(char))) < 0) {
            perror("read() failed");
            returnError(fd, 500, NULL); // Internal server error
            return NULL;
        }
        if(nbytes == 0) return NULL;
        buf[sizeof(buf) - sizeof(char)] = 0;
        char* end = strstr(buf, "\r\n");
        if(end != NULL) {
            int end_index = (int)(end - buf);
            strncat(request, buf, end_index);
            break;
        }
        strcat(request, buf);
    }

    // printf("Request: %s\n", request);

    // Check input and split to argv
    char** argv = splitstr(request, " ", 3);
    handle_request(fd, argv);
    freeargv(argv);

    // Close client socket
    close(fd);
    return NULL;
}

// ============================== M A I N ==============================
int main(int argc, char const *argv[])
{
    /* BRUTAL WAY TO FIX CHROME SIGPIPE */
    // signal(SIGPIPE, SIG_IGN);   // Ignore SIGPIPE

    // Making sure we got 3 arguments as needed
    if(argc != 4) usageMessage();

    int port = 0;
    int pool_size = 0;
    int max_requests = 0;

    // Making sure we got only (positive) numbers as arguments
    for(int i = 1; i < argc; i++) 
        if(isNumber(argv[i]) < 0) usageMessage();

    // Get the needed information from the arguments
    port = atoi(argv[1]);
    pool_size = atoi(argv[2]);
    max_requests = atoi(argv[3]);
    
    // Create the threadpool
    threadpool* pool = create_threadpool(pool_size);
    if(pool == NULL) usageMessage();

    // Socket
    int sd;     // Socket descriptor
    if((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket() failed");
        destroy_threadpool(pool);
        exit(EXIT_FAILURE);
    }

    // Bind
    struct sockaddr_in srv;     // Used by bind
    srv.sin_family = AF_INET;
    srv.sin_port = htons(port);
    srv.sin_addr.s_addr = htonl(INADDR_ANY);    // Client can connect to all of the server's addresses
    if(bind(sd, (struct sockaddr*)&srv, sizeof(srv)) < 0) {
        perror("bind() failed");
        close(sd);
        destroy_threadpool(pool);
        exit(EXIT_FAILURE);
    }

    // Listen
    if(listen(sd, LISTEN_Q_SIZE) < 0) {
        perror("listen() failed");
        close(sd);
        destroy_threadpool(pool);
        exit(EXIT_FAILURE);
    }

    // Loop
    for(int i = 0; i < max_requests; i++) {
        // Accept
        int newsd = accept(sd, NULL, NULL);
        if(newsd < 0) {
            perror("accept() failed");
            close(sd);
            destroy_threadpool(pool);
            exit(EXIT_FAILURE);
        }
        int* _newsd = (int*)malloc(sizeof(int));
        *_newsd = newsd;
        // Dispatch request to threadpool
        dispatch(pool, (void*)check_request, _newsd);
    }
    
    // Close server
    close(sd);
    // Destroy threadpool
    destroy_threadpool(pool);
    return 0;
}
