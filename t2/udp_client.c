#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<arpa/inet.h>]


//int main(int arg c, char* argv[]){
int main(){
    int sock_udp=socket(AF_INET,SOCK_DGRAM,0); //AF_INET: IPV4, SOCK_DGRAM: UDP
    socklen_t addr_size;
    char msg_sent[2000];
    char msg_received[2000];
    //creating IPV4 adress
    struct sockaddr_in to;
    struct sockaddr_in from;
    to.sin_family=AF_INET;
    to.sin_port=htons(1500);
    to.sin_addr.s_addr=htonl(INADDR_LOOPBACK); //same ip as the client
    while(1){
        gets(msg_sent);
        sendto(sock_udp, msg_sent,2000,0,&to,sizeof(to));
        recvfrom(sock_udp,msg_received,2000,0,(struct sockaddr*)&from,&addr_size);
        printf("got : %s\n",msg_received);        

    }
    return 0;
}