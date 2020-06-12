#include "douzo.h"
#include <unistd.h>
#include <stdio.h>

// #define _PRINT_REQUESTS_
#define PORT 8080

int main(int argc, char** argv){
	
	printf("> Starting server .. \n");
	
	int* socket = init_at(PORT);
	if(!socket){
		fprintf(stderr, "<!> Server didn't start !\n");
		return 1;
	} 
	printf("> Server is up ! \n");
	
	
	printf("> Listening at %d\n", PORT);
	
	int client;
	while(1){	
		REQUEST* req = wait_req(socket, &client);
		if(!req) continue;
		
		#ifdef _PRINT_REQUESTS_ /* For debugging purposes */
			printf("\n***\nMethod: %s\nRoute: %s\nHTTP version: %s\n",
				req->method,
				req->route,
				req->http_ver
			);
			HEADERS* header = req->headers;
			while(header){
				printf("%16s : %s\n",
					header->name,
					header->value
				);
				header = header->next_header;
			}
		#endif
				
		handle_req(client, req);
	}
	return 0;	
}
