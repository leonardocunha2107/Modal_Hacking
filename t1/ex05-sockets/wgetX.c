/**
 *  Jiazi Yi
 *
 * LIX, Ecole Polytechnique
 * jiazi.yi@polytechnique.edu
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "url.h"
#include "wgetX.h"

int main(int argc, char* argv[])
{

	url_info info;

	if (argc != 2) {
		exit_with_error("The wgetX must have exactly 1 parameter as input. \n");
	}
	char *url = argv[1];

	printf("Downloading %s \n", url);

	//get the url
	parse_url(url, &info);

	//	print_url_info(info);

	//download page
	char *recv_buf_t;
	recv_buf_t = malloc(sizeof(char)*(B_SIZE+1));
	memset(recv_buf_t,0, sizeof(char)*(B_SIZE+1));
	download_page(info, recv_buf_t);


	free(recv_buf_t);

	puts("the file is saved in received_page.");
	return (EXIT_SUCCESS);
}

void download_page(url_info info, char *recv_buf_t)
{
	int errcode, sock;
	//The code by Matheus Centa was heavily used here

	struct addrinfo *site , hints;

	sock=socket(AF_INET,SOCK_STREAM,0);

	//preprocessing for hints
	memset(&hints, 0, sizeof(hints));
	hints.ai_family= AF_INET;
	hints.ai_socktype=SOCK_STREAM;
	hints.ai_protocol=0;

	char port_s[5];
	sprintf(port_s,"%d",info.port);

	//getting adress from dns server
	errcode=getaddrinfo(info.host,port_s,&hints,&site);
	if(errcode<0)
		fprintf(stderr,"Error getting address\n");
	
	//connect to host
	errcode=connect(sock,site->ai_addr,sizeof(struct sockaddr));
	if(errcode<0)
		fprintf(stderr,"Error connecting\n");
	
	//send http request
	char* req_buf=http_get_request(info.path,info.host);
	write(sock,req_buf,strlen(req_buf));
//	keep_on=recv(sock,recv_buf_t,B_SIZE+1,0);

	// receiving response
	char* pack=recv_buf_t;
	int keep_on=1;
	while(keep_on){
		keep_on=recv(sock,pack,B_SIZE+1,0);
		pack+=strlen(pack);
	}
	read_http_reply(recv_buf_t);

	close(sock);
	
}

void write_data(const char * data)
{	
	FILE* output=fopen("received_data.html","w");
	fprintf(output,"%s",data);
	fclose(output);
}

char* http_get_request(char* path, char* host) {
	char * request_buffer = (char *) malloc(1024);
	memset(request_buffer, 0, sizeof(*request_buffer));
	snprintf(request_buffer, 1024, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",
			path, host);
	return request_buffer;
}


void read_http_reply(char* recv_buf_t) {
	//first line, get the status code
	char *status_line = strstr(recv_buf_t, "\r\n");
	*status_line = '\0';
	//		puts(recv_buf_t);
	char status[4];
	memcpy(status, recv_buf_t + 9, 3); //get the status string
	status[3] = '\0';
	int status_code = atoi(status);

	recv_buf_t = status_line + 2; //now move to the next line.
	char* page;

	url_info new_info;
	char* new_url;
	char* end_new_url;
	switch(status_code){
	case 200: //all ok
		page = strstr(recv_buf_t, "\r\n\r\n");
		*page = '\0';
		recv_buf_t = page + 4; //now the recv_buf pointer is pointing to the begin of the document
		write_data(recv_buf_t);
		break;
	case 301:
		new_url = strstr(recv_buf_t,"Location: ")+10;
		end_new_url=strstr(new_url,"\r\n");
		*end_new_url='\0';
		parse_url(new_url,&new_info);
		download_page(new_info,recv_buf_t);
		break;
	case 302:
		new_url = strstr(recv_buf_t,"Location: ")+10;
		char *end_new_url;
		end_new_url=strstr(new_url,"\r\n");
		*end_new_url='\0';
		parse_url(new_url,&new_info);
		download_page(new_info,recv_buf_t);
		break;

	}
}