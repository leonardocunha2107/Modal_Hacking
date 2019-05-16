#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){
    int sock_udp=socket(AF_INET,SOCK_DGRAM,0); //AF_INET: IPV4, SOCK_DGRAM: UDP
    char msg[2000];
    socklen_t addr_size;
     //creating IPV4 adress
    struct sockaddr_in in;
    struct sockaddr_in sender;
    in.sin_family=AF_INET;
    in.sin_port=htons(1500);
    in.sin_addr.s_addr=htonl(INADDR_LOOPBACK); //same ip as the client
    bind(sock_udp,&in, sizeof(in));
    while(1){
        recvfrom(sock_udp,msg,2000,0,&sender,&addr_size);
        printf("%s\n",msg);
        printf("%d\n",ntohs(sender.sin_port));
        sendto(sock_udp,msg,strlen(msg)+1,0,&sender,sizeof(sender));
    }
}