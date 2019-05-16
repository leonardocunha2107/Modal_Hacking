#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 100



int sock_udp;
void *process(void* sender){
    sleep(3);
    struct sockaddr_in sock= *(struct sockaddr_in*)sender;
    //printf("%d\n",ntohs(sock.sin_port));
    char* msg[100];
    strcpy(msg,"Received your message and processed it");
    sendto(sock_udp,msg,200,0,&sock,sizeof(struct sockaddr_in));
    free(sender);
}


int main(){
    pthread_t threads[NTHREADS];

    socklen_t addr_size=sizeof(struct sockaddr_in);
    struct sockaddr_in sender;

    sock_udp=socket(AF_INET,SOCK_DGRAM,0); //AF_INET: IPV4, SOCK_DGRAM: UDP
    char msg[2000];
    
     //creating IPV4 adresses
    struct sockaddr_in sock;
    sock.sin_family=AF_INET;
    sock.sin_port=htons(1500);
    sock.sin_addr.s_addr=htonl(INADDR_LOOPBACK); //same ip as the client



    bind(sock_udp,&sock,sizeof(sock));
    while(1){
        recvfrom(sock_udp,msg,100,0,&sender,&addr_size);
        puts(msg);
        struct sockaddr_in* send_to=malloc(sizeof(sender));
        memcpy(send_to,&sender,sizeof(sender));
        

        pthread_create(threads+i,NULL,process,send_to);

    }

}





	//destination