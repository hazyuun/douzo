#include "douzo.h"
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

bool 	PRINT_REQUESTS 	= false;
int 	PORT 			= 8080;


#define NORMAL 		"\e[0m"
#define ERROR 		"\e[1;31m"
#define WARNING 	"\e[1;33m"
#define SUCCESS 	"\e[1;32m"
#define HIGHLIGHT 	"\e[1;36m"


int main(int argc, char** argv){
   int option;
   while((option = getopt(argc, argv, ":rp:h")) != -1){
      switch(option){
         case 'r':
         	PRINT_REQUESTS = true;
            printf(HIGHLIGHT"NOTE : Requests will be printed\n");
            break;
         case 'p':
            PORT = atoi(optarg);
			if(PORT < 1024 || PORT > 65535){
				printf(ERROR"> Invalid port !");
				return 1;	
			}
            break;
         case ':':
            printf(ERROR"Please specify the port after -%c\n", optopt);
            return 1;
            break;
         case 'h':
         case '?': 
            printf(NORMAL"Usage: %s [-r] [-p port]\n", argv[0]);
            printf("\t-r : Print http requests\n");
            printf("\t-p : Use the specified port instead of default value (8080)\n");
         	return 1;
            break;
      }
   }
	printf(NORMAL"> Starting server .. \n");
	
	int* socket = init_at(PORT);
	if(!socket){
		fprintf(stderr, ERROR"<!> Server didn't start !\n");
		return 1;
	} 
	printf(SUCCESS"> Server is up ! \n");
	
	
	printf(HIGHLIGHT"> Listening at %d\n", PORT);
	printf(NORMAL"\n");
	int client;
	while(1){	
		REQUEST* req = wait_req(socket, &client);
		if(!req) continue;
		
		if(PRINT_REQUESTS){ /* For debugging purposes */
			printf("\n***\nMethod: %s\nRoute: %s\nHTTP version: %s\n",
				req->method,
				req->route,
				req->http_ver
			);
			HEADERS* header = req->headers;
			while(header){
				printf(HIGHLIGHT"%32s :"NORMAL" %s\n",
					header->name,
					header->value
				);
				header = header->next_header;
			}
		}
		
		struct handle_req_args_t args;
		args.client = client;
		args.req = req;
		
		pthread_t thread;
		pthread_create(&thread, NULL, &handle_req_threaded, (void*)(&args));
		pthread_join(thread, NULL);
	}
	return 0;	
}
