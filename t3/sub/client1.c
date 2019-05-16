#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


//int main(int arg c, char* argv[]){
int main(){
    int sock_udp=socket(AF_INET,SOCK_DGRAM,0); //AF_INET: IPV4, SOCK_DGRAM: UDP
    char msg[2000];
    int len;

    //creating IPV4 adress
    struct sockaddr_in to;
    to.sin_family=AF_INET;
    to.sin_port=htons(1500);
    to.sin_addr.s_addr=htonl(INADDR_LOOPBACK); //same ip as the client
    while(1){
        gets(msg);
        len=strlen(msg)+1;
        sendto(sock_udp, msg,len,0,&to,sizeof(to));

    }
    return 0;
}