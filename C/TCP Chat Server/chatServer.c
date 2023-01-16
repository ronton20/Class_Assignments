#include "chatServer.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <bits/socket.h>

static int end_server = 0;

void intHandler(int SIG_INT) {
	/* use a flag to end_server to break the main loop */
    end_server = 1;
}

/* 
 * Check if a string is a number.
 */
int isNumber(char *str) {
    int i;
    for (i = 0; i < strlen(str); i++) {
        if (str[i] < '0' || str[i] > '9') {
            return 0;
        }
    }
    return 1;
}

int main (int argc, char *argv[])
{
	
	signal(SIGINT, intHandler);

    // Check command line arguments
    if(argc != 2 || !isNumber(argv[1])) {
        printf("Usage: server <port>");
        exit(EXIT_FAILURE);
    }

    // Get port number and validate it
    int port = atoi(argv[1]);
    if(port < 1 || port > 65535) {
        printf("Usage: server <port>");
        exit(EXIT_FAILURE);
    }
   
	conn_pool_t* pool = malloc(sizeof(conn_pool_t));
	init_pool(pool);
   
	/*************************************************************/
	/* Create an AF_INET stream socket to receive incoming      */
	/* connections on                                            */
	/*************************************************************/
	int sd;     // Socket descriptor
    if((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket() failed");
        free(pool);
        exit(EXIT_FAILURE);
    }
   
   
	/*************************************************************/
	/* Set socket to be nonblocking. All of the sockets for      */
	/* the incoming connections will also be nonblocking since   */
	/* they will inherit that state from the listening socket.   */
	/*************************************************************/
	int on = 1;
    int rc = ioctl(sd, (int)FIONBIO, (char *)&on);
    if(rc < 0) {
        perror("ioctl() failed");
        close(sd);
        free(pool);
        exit(EXIT_FAILURE);
    }

	/*************************************************************/
	/* Bind the socket                                           */
	/*************************************************************/
	struct sockaddr_in srv;     // Used by bind
    srv.sin_family = AF_INET;
    srv.sin_port = htons(port);
    srv.sin_addr.s_addr = htonl(INADDR_ANY);    // Client can connect to all of the server's addresses
    if(bind(sd, (struct sockaddr*)&srv, sizeof(srv)) < 0) {
        perror("bind() failed");
        close(sd);
        free(pool);
        exit(EXIT_FAILURE);
    }

	/*************************************************************/
	/* Set the listen back log                                   */
	/*************************************************************/
	if(listen(sd, 5) < 0) {
        perror("listen() failed");
        close(sd);
        free(pool);
        exit(EXIT_FAILURE);
    }

	/*************************************************************/
	/* Initialize fd_sets  			                             */
	/*************************************************************/
    FD_SET(sd, &pool->read_set);
    pool->maxfd = sd;
    pool->nr_conns = 0;

	/*************************************************************/
	/* Loop waiting for incoming connects, for incoming data or  */
	/* to write data, on any of the connected sockets.           */
	/*************************************************************/
	do
	{
		/**********************************************************/
		/* Copy the master fd_set over to the working fd_set.     */
		/**********************************************************/
        pool->ready_read_set = pool->read_set;
        pool->ready_write_set = pool->write_set;

		/**********************************************************/
		/* Call select() 										  */
		/**********************************************************/
		printf("Waiting on select()...\nMaxFd %d\n", pool->maxfd);
		if((pool->nready = select(pool->maxfd + 1, &pool->ready_read_set, &pool->ready_write_set, 0, 0)) < 0) {
            perror("select() failed");
            end_server = 1;
            break;
        }
		
		/**********************************************************/
		/* One or more descriptors are readable or writable.      */
		/* Need to determine which ones they are.                 */
		/**********************************************************/
		
        // Skip STDIN STDOUT and STDERR
		for (int i = 3; i <= pool->maxfd && pool->nready > 0; i++)
		{
			/* Each time a ready descriptor is found, one less has  */
			/* to be looked for.  This is being done so that we     */
			/* can stop looking at the working set once we have     */
			/* found all of the descriptors that were ready         */
				
			/*******************************************************/
			/* Check to see if this descriptor is ready for read   */
			/*******************************************************/
			if (FD_ISSET(i, &pool->ready_read_set))
			{
				/***************************************************/
				/* A descriptor was found that was readable		   */
				/* if this is the listening socket, accept one      */
				/* incoming connection that is queued up on the     */
				/*  listening socket before we loop back and call   */
				/* select again. 						            */
				/****************************************************/
                if(i == sd) {
                    int new_sd = accept(sd, NULL, NULL);
                    if(new_sd < 0) {
                        pool->nready--;
                        continue;
                    }
                    printf("New incoming connection on sd %d\n", new_sd);
                    add_conn(new_sd, pool);
                    pool->nready--;
                    continue;
                }
				/****************************************************/
				/* If this is not the listening socket, an 			*/
				/* existing connection must be readable				*/
				/* Receive incoming data his socket             */
				/****************************************************/
				// Read header from client
                printf("Descriptor %d is readable\n", i);
                char buf[BUFFER_SIZE] = "";
                char message[BUFFER_SIZE] = "";
                int bytesRead = 0;
                int nbytes = 0;
                while(1) {
                    if((nbytes = read(i, buf, sizeof(buf) - 1)) < 0) {
                        break;
                    }
                    if(nbytes == 0) {
                        // Connection closed
                        /* If the connection has been closed by client 		*/
                        /* remove the connection (remove_conn(...))    		*/
                        remove_conn(i, pool);
                        if(pool->maxfd == 0) pool->maxfd = sd;
                        printf("Connection closed for sd %d\n",i);
                        break;
                    }
                    buf[sizeof(buf) - 1] = 0;
                    char* end = strstr(buf, "\r\n");
                    if(end != NULL) {
                        int end_index = (int)(end + 2 - buf);
                        strncat(message, buf, end_index);
                        break;
                    }
                    strcat(message, buf);
                    bytesRead += nbytes;
                }
                printf("%d bytes received from sd %d\n", bytesRead, sd);
							
				/**********************************************/
				/* Data was received, add msg to all other    */
				/* connectios					  			  */
				/**********************************************/
                add_msg(i, message, strlen(message), pool);
                pool->nready--;
				
			} /* End of if (FD_ISSET()) */
			/*******************************************************/
			/* Check to see if this descriptor is ready for write  */
			/*******************************************************/
			if (FD_ISSET(i, &pool->ready_write_set)) {
				/* try to write all msgs in queue to sd */
				write_to_client(i, pool);
                pool->nready--;
		 	}
		 /*******************************************************/
		 
		 
      } /* End of loop through selectable descriptors */

   } while (end_server == 0);

	/*************************************************************/
	/* If we are here, Control-C was typed,						 */
	/* clean up all open connections					         */
	/*************************************************************/
    printf("Cleaning up connections...\n");
    conn_t* conn = pool->conn_head;
    while(conn != NULL) {
        conn_t* next = conn->next;
        remove_conn(conn->fd, pool);
        conn = next;
    }
    close(sd);
    free(pool);
    return 0;
}

int init_pool(conn_pool_t* pool) {
    if(pool == NULL) return -1;
	//initialized all fields
    pool->maxfd = 0;
    pool->nready = 0;
    FD_ZERO(&pool->read_set);
    FD_ZERO(&pool->ready_read_set);
    FD_ZERO(&pool->write_set);
    FD_ZERO(&pool->ready_write_set);
    pool->nr_conns = 0;
    pool->conn_head = NULL;
	return 0;
}

int add_conn(int sd, conn_pool_t* pool) {
    if(pool == NULL || sd < 0) return -1;
	// 1. allocate connection and init fields
    conn_t *new_conn = (conn_t*)malloc(sizeof(conn_t));
    if(new_conn == NULL) {
        perror("malloc() failed");
        return -1;
    }

    new_conn->fd = sd;
    new_conn->write_msg_head = NULL;
    new_conn->write_msg_tail = NULL;

    // Add to fd_set
    FD_SET(sd, &pool->read_set);

    // 2. add connection to pool
    conn_t *curr = pool->conn_head;
    // If list is empty add to head
    if(curr == NULL) {
        pool->conn_head = new_conn;
        new_conn->prev = NULL;
    } else {
        // Find last connection
        while(curr->next != NULL) {
            curr = curr->next;
        }
        // Add new connection to end of list
        curr->next = new_conn;
        new_conn->prev = curr;
    }
    new_conn->next = NULL;

    // Update max fd
    if(sd > pool->maxfd) {
        pool->maxfd = sd;
    }
    // Update number of connections
    pool->nr_conns++;
	return 0;
}

int remove_conn(int sd, conn_pool_t* pool) {
    if(pool == NULL || sd < 0) return -1;
    printf("removing connection with sd %d \n", sd);
    // 1. remove connection from pool 
    // Find connection
    conn_t *curr = pool->conn_head;
    while(curr != NULL && curr->fd != sd) {
        curr = curr->next;
    }
    if(curr == NULL) {
        // Connection not found
        return -1;
    }
    // Remove from list
    if(curr->prev != NULL) { curr->prev->next = curr->next; }
    else { pool->conn_head = curr->next; }
    if(curr->next != NULL) { curr->next->prev = curr->prev; }
    pool->nr_conns--;

	// 2. deallocate connection 
    // Free all messages
    msg_t *msg_curr = curr->write_msg_head;
    while(msg_curr != NULL) {
        msg_t *tmp = msg_curr;
        msg_curr = msg_curr->next;
        if(tmp->message != NULL) free(tmp->message);
        free(tmp);
    }
    // Free connection
    free(curr);

	// 3. remove from sets 
    FD_CLR(sd, &pool->read_set);
    FD_CLR(sd, &pool->write_set);

    // 4. update max_fd if needed 
    if(sd == pool->maxfd) {
        pool->maxfd = 0;
        curr = pool->conn_head;
        while(curr != NULL) {
            if(curr->fd > pool->maxfd) {
                pool->maxfd = curr->fd;
            }
            curr = curr->next;
        }
    }

    close(sd);
	return 0;
}

int add_msg(int sd,char* buffer,int len,conn_pool_t* pool) {
	if(pool == NULL || sd < 0) return -1;
    if(buffer == NULL || len <= 0) return -1;
	// 1. add msg_t to write queue of all other connections
    conn_t *curr = pool->conn_head;
    while(curr != NULL) {
        if(curr->fd != sd) {
            // Allocate new msg
            struct msg *new_msg = (struct msg*)malloc(sizeof(struct msg));
            if(new_msg == NULL) {
                perror("malloc() failed");
                return -1;
            }
            // Copy buffer
            new_msg->message = (char*)malloc(len);
            if(new_msg->message == NULL) {
                perror("malloc() failed");
                return -1;
            }
            memcpy(new_msg->message, buffer, len);
            new_msg->size = len;
            new_msg->next = NULL;
            // Add to write queue
            // If queue is empty add to head
            if(curr->write_msg_head == NULL) {
                curr->write_msg_head = new_msg;
                curr->write_msg_tail = new_msg;
                new_msg->prev = NULL;
            } else {
                curr->write_msg_tail->next = new_msg;
                new_msg->prev = curr->write_msg_tail;
                curr->write_msg_tail = new_msg;
            }
	        // 2. set each fd to check if ready to write
            FD_SET(curr->fd, &pool->write_set);
        }
        curr = curr->next;
    }
	
	return 0;
}

int write_to_client(int sd,conn_pool_t* pool) {
    if(pool == NULL || sd < 0) return -1;
	// 1. write all msgs in queue
    conn_t *curr = pool->conn_head;
    while(curr != NULL) {
        if(curr->fd == sd) {
            // Write all messages
            msg_t *msg_curr = curr->write_msg_head;
            while(msg_curr != NULL) {
                int bytes_written = write(sd, msg_curr->message, msg_curr->size);
                if(bytes_written < 0) {
                    perror("write() failed");
                    return -1;
                }
                // Remove message from queue
                if(msg_curr->prev != NULL) { msg_curr->prev->next = msg_curr->next; }
                else { curr->write_msg_head = msg_curr->next; }
                if(msg_curr->next != NULL) { msg_curr->next->prev = msg_curr->prev; }
                else { curr->write_msg_tail = msg_curr->prev; }

                // 2. deallocate each writen msg 
                if(msg_curr->message != NULL) free(msg_curr->message);
                free(msg_curr);
                msg_curr = curr->write_msg_head;
            }
            // 3. if all msgs were writen successfully, there is nothing else to write to this fd...
            // Remove from write set
            FD_CLR(sd, &pool->write_set);
            break;
        }
        curr = curr->next;
    }
	return 0;
}
