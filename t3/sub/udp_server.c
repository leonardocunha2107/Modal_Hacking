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
void *process(){
    sleep(5);
    puts("I'm done");
}

int main(){
    int sock_udp;
    pthread_t threads[NTHREADS];

    void** a=malloc(4);
    *a=NULL;

    sock_udp=socket(AF_INET,SOCK_DGRAM,0); //AF_INET: IPV4, SOCK_DGRAM: UDP
    char msg[2000];
    
     //creating IPV4 adresses
    struct sockaddr_in sock;
    sock.sin_family=AF_INET;
    sock.sin_port=htons(1500);
    sock.sin_addr.s_addr=htonl(INADDR_LOOPBACK); //same ip as the client



    bind(sock_udp,&sock,sizeof(sock));
    //pthread_create(t+i,NULL,increase_counter,&mutex);
    int i=0;
    while(1){
        recvfrom(sock_udp,msg,100,0,NULL,0);
        pthread_create(threads+i,NULL,process,NULL);

    }

    pthread_join(threads[NTHREADS-1],a);
}



