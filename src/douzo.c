#include "douzo.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>

#include "http.h"

int* init_at(int port){
	
	int* sock_fd;
	*sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in serv_addr;
	bzero((char *) &serv_addr, sizeof(serv_addr));
	
	serv_addr.sin_family  	  	= AF_INET;
	serv_addr.sin_port	  		= htons(port);
	serv_addr.sin_addr.s_addr 	= INADDR_ANY;

	bind(
	  *sock_fd,
	  (struct sockaddr*) &serv_addr,
	  sizeof(serv_addr)
	    );
	listen(*sock_fd, 5);
	return sock_fd;
}

REQUEST* wait_req(int* sock_fd, int* cl_sock_fd){
	struct sockaddr_in cli_addr;
	int len = sizeof(cli_addr);
	int client_sock_fd = accept(
				*sock_fd,
		       	(struct sockaddr *) &cli_addr,
		       	&len
			   );
	char* req = (char*) malloc(1024);
	bzero(req, 1024);
	read(client_sock_fd, req, 1024);
	*cl_sock_fd = client_sock_fd; 
	return parse_req(req);
}

char* get_file(char* path){
	FILE *f;
	long size;
	char *buffer;
	
	f = fopen (path , "r");
	if(!f) return NULL;
	
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	rewind(f);
	
	buffer = calloc(1, size+1);
	if(!buffer) {
		fclose(f);
		fputs("ERR WHEN ALLOCATING MEMORY",stderr);

		return NULL;
	}
	
	if(fread(buffer, size, 1, f)!=1){
		  free(buffer);
		  fclose(f);
		  //fputs("ERR WHEN READING FILE",stderr);
		  return NULL;
	}
	fclose(f);
	return buffer;
}

void handle_req(int sock_fd, REQUEST* req){
	if(!req) return;
	char* res = (char*) malloc(2048*2);
	strcat(res, "HTTP/1.1 ");
	if(!strcmp(req->method, "GET")){
		
		char file_path[2048];
		getcwd(file_path, sizeof(file_path));
		strcat(file_path, req->route);
		char* body = get_file(file_path);
		if(!body){
			body = _404_page;
			strcat(res, Not_Found);
		}else
			strcat(res, OK);

		strcat(res, "\r\nContent-Type: text/html");

		char tmp[512];
		sprintf(tmp, "\r\nContent-Length: %d\r\n\r\n", strlen(body));
		strcat(res, tmp);

		strcat(res, body);
		
	
	}else{
		strcat(res, Not_Implemented);
		strcat(res, "\r\nContent-Type: text/html\r\n\r\n");
		strcat(res, "<h1>Not implemented<h1>");
	}
	send(sock_fd, res, strlen(res), 0);
}

