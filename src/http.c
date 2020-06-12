#include "http.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Takes a raw http request, and parses it to a struct REQUEST */
REQUEST* parse_req(const char* request_str){
	char* rq_tmp;
	if(request_str == NULL || request_str[0] == '\0')
		return NULL;
	strcpy(rq_tmp, request_str); 
	REQUEST* parsed = (REQUEST*) malloc(sizeof(REQUEST));
	if(!parsed)
		return NULL;
	
	memset(parsed, 0, sizeof(REQUEST));	
	char* rq_save; // for strtok_r 
	char* row = strtok_r(rq_tmp, "\n", &rq_save);
	
	/* The method, the route, and the http version */
	char* row_save; /* for strtok_r */
	parsed->method		= strtok_r(row, " ", &row_save);  //TODO: check if it is a valid method
	parsed->route		= strtok_r(NULL, " ", &row_save);
	parsed->http_ver	= strtok_r(NULL, " ", &row_save); //TODO: check if it is 1.1
	
	/* headers */
	HEADERS* headers = NULL;	
	HEADERS* helper = NULL;
	while(memcmp(row = strtok_r(NULL, "\n", &rq_save), "\r", 1))
	{	
		helper = headers;
		
		headers = (HEADERS*) malloc(sizeof(HEADERS));
		memset(headers, 0, sizeof(HEADERS));
		headers->name = strtok_r(row, ":", &row_save);
		headers->value = strtok_r(NULL, "\r", &row_save);
		headers->next_header = helper;	
	}
	parsed->headers = headers;
	
	parsed->body 	= strtok_r(NULL, "\0", &rq_save);
	return parsed;
}

/* DEALLOCATORS */
void del_request(REQUEST* req){
	if(!req) return;
	free(req->method);
	free(req->route);
	free(req->http_ver);
	free(req->body);
	del_header(req->headers);
	free(req);
}
void del_header (HEADERS* header){
	if(!header) return;
	free(header->name);
	free(header->value);
	del_header(header->next_header);
	free(header);
}
void del_response(RESPONSE* res){
	if(!res) return;
	free(res->status);
	free(res->body);
	del_header(res->headers);
	free(res);
}