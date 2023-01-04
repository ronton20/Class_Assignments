#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#define MAX_LEN 1024  //max string length
#define MAX_PORT_SIZE 65536  //max port size (2^16)

char getType(const char* arg);
int isNumber(const char* str);
size_t handleURL(const char* URL, char* host, char* path);
int handleP(const char* argv, char* content, int content_length);
int handleR(const char* argv[], int index, int num_of_args,char* arguments);
void usageMessage();


/**
 * checks for the type of argument it is,
 * returns 'p' for -p, 'r' for -r, 'u' for <URL> or 0 for none of the above
  \param arg the argument to check
*/
char getType(const char* arg) {
  if(arg == NULL) return 0;
  if(strlen(arg) <= 1) return 0;
  if(arg[0] != '-') return 'u';
  switch (arg[1]) {
    case 'p':
      return 'p';
    case 'r':
      return 'r';
    default:
      return 0;
  }
}

/**
 * parses the URL and extracts the host, path, and port number. It returns the port number or -1 if the URL is invalid.
  \param URL URL to handle
  \param host host string
  \param path path string
*/
size_t handleURL(const char* URL, char* host, char* path) {
  if(URL == NULL || host == NULL || path == NULL) return -1;
  
  size_t port;
  char prefix[] = "http://";
  size_t prefixLen = strlen(prefix);
  size_t URLlength = strlen(URL);
  if(URLlength < prefixLen) return -1;

  //checking if the URL starts with 'http://'
  if(strncmp(URL, prefix, prefixLen) != 0) return -1;

  //getting pointers to the port and the path
  size_t portIndex = prefixLen + strcspn(URL + prefixLen, ":");
  size_t pathIndex = prefixLen + strcspn(URL + prefixLen, "/");

  //checking for a given port and assigning it, otherwise port = 80
  if(portIndex < pathIndex) {
    char port_str[MAX_LEN] = "";
    //getting the port string out of the URL
    strncpy(port_str, URL + portIndex + 1, pathIndex - (portIndex + 1));
    //checking if the port has only numbers inside of it
    if(isNumber(port_str) == -1) return -1;
    //parsing it to integer and saving it as the port to return
    port = atoi(port_str);
    if(port > MAX_PORT_SIZE) return -1;

    //getting the host out of the URL
    char host_url[MAX_LEN] = "";
    strncpy(host_url, URL + prefixLen, portIndex - prefixLen);
    strcat(host, host_url);
  } 
  else if(portIndex < URLlength) { return -1; }
  else { 
    port = 80;
    //getting the host out of the URL
    char host_url[MAX_LEN] = "";
    strncpy(host_url, URL + prefixLen, pathIndex - prefixLen);
    strcat(host, host_url);
  }

  //saving the path
  char new_path[MAX_LEN] = "/";
  if(pathIndex < URLlength) 
    strncpy(new_path, URL + pathIndex, URLlength - pathIndex);
  
  strcat(path, new_path);

  return port;
}

/**
 * handles -p arguments, returns 0 on success, -1 on failure
  \param argv string to add content from
  \param content content string
  \param content_length size of the content to add
*/
int handleP(const char* argv, char* content, int content_length) {
  // if(content_length <= 0) return -1;
  if(content_length <= 0 || strlen(argv) < content_length) return -1;

  strncpy(content, argv, content_length);
  return 0;
}

/**
 * handles -r arguments, returns 0 on success, -1 on failure
  \param argv array of arguments to add from
  \param index index of first argument
  \param num_of_args number of arguments to add
  \param arguments string of the translated arguments to add to the header
*/
int handleR(const char* argv[], int index, int num_of_args,char* arguments) {
  if(argv == NULL || index < 0 || num_of_args < 0 || arguments == NULL) return -1;
  if(num_of_args == 0) return 0;
  
  char toAdd[MAX_LEN] = "";
  int new = 0;
  if(strlen(arguments) == 0) {
    strcat(toAdd, "?");
    new = 1;
  }
  for (int i = 0; i < num_of_args; i++) {
    if(argv[index + i] == NULL) return -1;
    //making sure '?' and '&' are not in the argument
    if(strstr(argv[index + i], "?") != NULL || strstr(argv[index + i], "&") != NULL) return -1;
    if(!(new == 1 && i == 0)) strcat(toAdd, "&");  // <---- adding '&' to seperate arguments

    size_t equals_index = strcspn(argv[index + i], "=");
    //checking if there is no '=' in the argument
    if(equals_index <= 0 || equals_index >= strlen(argv[index + i])) return -1;
    strcat(toAdd, argv[index + i]);
  }
  strcat(arguments, toAdd);
  return 0;
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

//prints a usage mesage
void usageMessage() { printf("Usage: client [-p n <text>] [-r n < pr1=value1 pr2=value2 ...>] <URL>\n"); }


//============================== MAIN ==============================
int main(int argc, char const *argv[])
{
  char request[MAX_LEN] = "";
  char head[MAX_LEN] = "GET";
  char content[MAX_LEN] = "";
  char arguments[MAX_LEN] = "";
  char path[MAX_LEN] = "";
  char host[MAX_LEN] = "";
  char protocol[] = "HTTP/1.0";
  size_t port = 0;
  int content_length = -1;

  for (int i = 1; i < argc; i++) {
    char type = getType(argv[i]);
    switch(type) {
      //==================== URL ====================
      case 'u':
        port = handleURL(argv[i], host, path);
        if(port == -1) {
          usageMessage();
          exit(EXIT_FAILURE);
        }
        break;

      //==================== -p ====================
      case 'p':
        if(isNumber(argv[++i]) == -1) {
          usageMessage();
          exit(EXIT_FAILURE);
        }
        content_length = atoi(argv[i]);
        if(handleP(argv[++i], content, content_length) == -1) {
          usageMessage();
          exit(EXIT_FAILURE);
        }
        strcpy(head, "POST"); // <---- changing 'GET' to 'POST' for the header
        break;

      //==================== -r ====================
      case 'r':
      if(isNumber(argv[++i]) == -1) {
          usageMessage();
          exit(EXIT_FAILURE);
        }
        int num_of_args = atoi(argv[i++]);
        if(handleR(argv, i, num_of_args, arguments) == -1) {
          usageMessage();
          exit(EXIT_FAILURE);
        }
        i += (num_of_args - 1);
        break;
      
      //no match found in string
      default:
        usageMessage();
        exit(EXIT_FAILURE);
    }
  }

  if(strcmp(host, "") == 0) {
    usageMessage();
    exit(EXIT_FAILURE);
  }

  //============================== setting up the request ==============================
  strcat(request, head);  // <--- GET / POST
  strcat(request, " ");
  strcat(request, path);  // <--- path inside host domain
  strcat(request, arguments); // <--- all the arguments
  strcat(request, " ");
  strcat(request, protocol);  // <--- HTTP/1.0
  strcat(request, "\r\n");
  strcat(request, "Host: ");  // <--- Host:
  strcat(request, host);  // <--- <host domain>
  strcat(request, "\r\n");
  //adding content if we had any:
  if(content_length > 0) {
    char body[MAX_LEN] = "";
    sprintf(body, "Content-length:%d\r\n\r\n", content_length);
    strcat(body, content);
    strcat(request, body);
  }
  strcat(request, "\r\n");

  int request_len = strlen(request);
  int request_size = sizeof(request);
  printf("HTTP request =\n%s\nLEN = %d\n", request, request_len);

  //============================== Talking to the Server ==============================
  int fd;
  struct sockaddr_in srv;

  //create an internet socket
  if((fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket() failed\n");
    exit(EXIT_FAILURE);
  }
  
  //create sockaddr instance
  struct hostent* hp;
  if((hp = gethostbyname(host)) == NULL) {
    herror("gethostbyname() failed\n");
    exit(EXIT_FAILURE);
  }
  srv.sin_family = PF_INET;
  srv.sin_addr.s_addr = ((struct in_addr*)(hp->h_addr))->s_addr;
  srv.sin_port = htons(port);

  //connect to the socket
  if(connect(fd, (struct sockaddr*)&srv, sizeof(srv)) < 0) {
    perror("connect() failed\n");
    exit(EXIT_FAILURE);
  }

  char buf[MAX_LEN] = "";
  int nbytes = 0;

  //sending the request
  if((nbytes = write(fd, request, request_size)) < 0) {
    perror("write() failed\n");
    exit(EXIT_FAILURE);
  }

  int size = 0;
  //accepting reply
  while (1){
    if((nbytes = read(fd, buf, sizeof(buf) - sizeof(char))) < 0) {
      perror("\nread() failed\n");
      exit(EXIT_FAILURE);
    }
    if(nbytes == 0) break;
    buf[sizeof(buf) - sizeof(char)] = 0;
    printf("%s", buf);
    size += nbytes;
  }
  close(fd);
  
  printf("\n Total received response bytes: %d\n",size);
  
  return 0;
}
