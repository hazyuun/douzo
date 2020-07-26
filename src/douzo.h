#ifndef _DOUZO__H_
#define _DOUZO__H_

#include "http.h"
#include <stdbool.h>

#define _404_page "<h1>404 Not Found</h1>" \
				  "<hr />"\
				  "<p>Douzo server | made by A-Rain-Lover (Yuun)</p>"

/* Creates a socket, binds it, and listens at the given port. */
/* Returns a file descriptor to that socket. 				 */
int* init_at(int port);


/* Waits for a client to connect and send a request.  	 */
/* Returns the parsed request								 */
REQUEST* wait_req(int* sock_fd, int* cl_sock_fd);


/* Returns a file's contents as a char*, given its path */
char* get_file(char* path);

/* Takes a parsed request, handles it and send the response */
void handle_req(int sock_fd, REQUEST* req);

struct handle_req_args_t{
	int 		client;
	REQUEST* 	req;	
};

void* handle_req_threaded(void* args);

#endif
