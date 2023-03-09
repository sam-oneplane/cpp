#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <malloc.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#include <arpa/inet.h>

#include <linux/if_packet.h>

#include <net/if.h> // network config
#include <netinet/in.h> // raw socket snd package
#include <netinet/ip.h>
#include <netinet/if_ether.h> // raw socket data 
#include <netinet/udp.h> // transport layer config
#include <netinet/tcp.h>

typedef struct {
    sockaddr_in source;
    sockaddr_in dest;
}Addr ;


struct Data {

    unsigned char *data;
    int bufflen ;
    int remianing_data;

    Data(const int len) : bufflen(len) {};

    void extract_udp(Transport& trans, unsigned char *packetbuff) {
        data = packetbuff + sizeof(struct ethhdr) + trans.iphdrlen + sizeof(struct udphdr);
        remianing_data = bufflen - (trans.iphdrlen + sizeof(struct ethhdr) + sizeof(struct udphdr));
    }

    void log() {
        std::cout << "Data : \n";
        for (int i = 0; i < remianing_data; i++) {
            if(i != 0 && (i % 16) == 0) {
                printf("%.2x\n", data[i]);
            }
        }

    }

};


struct Transport{

    struct udphdr *udp;
    struct tcphdr *tcp;
    unsigned int iphdrlen;

    void set_header_len(struct iphdr *ip) {
        // move pointer to tcp/udp header by calc internet header length
        iphdrlen = ip->ihl*4;
    }

    void extract_udp(unsigned char *packetbuff) {
        udp = reinterpret_cast<struct udphdr *>(packetbuff + sizeof(struct ethhdr) + iphdrlen);

    }

    void extract_tcp(unsigned char *packetbuff) {
        tcp = reinterpret_cast<struct tcphdr *>(packetbuff + sizeof(struct ethhdr) + iphdrlen);
    }

    void log_udp() {
        
        printf("\t|-Source Port : %d\n" , ntohs(udp->source));
        printf("\t|-Destination Port : %d\n" , ntohs(udp->dest));
        printf("\t|-UDP Length : %d\n" , ntohs(udp->len));
        printf("\t|-UDP Checksum : %d\n" , ntohs(udp->check));
    }
};

struct IP {

    struct iphdr *ip ;
    Addr *ipaddr;

    void extract(unsigned char *packetBuff) {
        ip =  reinterpret_cast<struct iphdr *>(packetBuff + sizeof(struct ethhdr));
        memset(&(ipaddr->source), 0 , sizeof(ipaddr->source)); 
        (ipaddr->source).sin_addr.s_addr = ip->saddr;
        memset(&(ipaddr->dest), 0 , sizeof(ipaddr->dest));
        (ipaddr->dest).sin_addr.s_addr = ip->daddr;
    }

    void log() {
        printf("\t|-Version : %d\n",(unsigned int)ip->version);
        printf("\t|-Internet Header Length : %d DWORDS or %d Bytes\n",(unsigned int)ip->ihl,((unsigned int)(ip->ihl))*4);
        printf("\t|-Type Of Service : %d\n",(unsigned int)ip->tos);
        printf("\t|-Total Length : %d Bytes\n",ntohs(ip->tot_len));
        printf("\t|-Identification : %d\n",ntohs(ip->id));
        printf("\t|-Time To Live : %d\n",(unsigned int)ip->ttl);
        printf("\t|-Protocol : %d\n",(unsigned int)ip->protocol);
        printf("\t|-Header Checksum : %d\n",ntohs(ip->check));
        printf("\t|-Source IP : %s\n", inet_ntoa((ipaddr->source).sin_addr));
        printf("\t|-Destination IP : %s\n",inet_ntoa((ipaddr->dest).sin_addr));
    }

};


struct Header {
    struct ethhdr *eth; // define at if_hether.h
    
    /* data */
    void extract(unsigned char *packetBuff){
        eth = reinterpret_cast<struct ethhdr *>(packetBuff);
    }

    void log() {
        std::cout << "\nEthernet Header\n";
        printf("\t|-Source Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n", 
                    eth->h_source[0],eth->h_source[1],eth->h_source[2],
                    eth->h_source[3],eth->h_source[4],eth->h_source[5]);
        printf("\t|-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",
                    eth->h_dest[0],eth->h_dest[1],eth->h_dest[2],eth->h_dest[3],
                    eth->h_dest[4],eth->h_dest[5]);
        std::cout << "\tProtocol :" << eth->h_proto << std::endl;
        // h_proto gives info about the next level
    }
};

template <int BUFFER_SIZE, int PROTOCOL>
struct RawSocket {

    int sock_r;
    unsigned char *packet_r;
    struct sockaddr saddr;
    Header header;
    IP iprot;
    Transport tcp;
    Addr target;

    RawSocket() {
        /*  protocol types :
            rcv : ETH_P_ALL , ETH_P_IP 
            snd : IPPROTO_RAW
        */
        sock_r = socket(AF_PACKET, SOCK_RAW, htons(PROTOCOL)); 
    }

    int chackPacket() {
        if (sock_r < 0 )
            return EXIT_FAILURE;
        return EXIT_SUCCESS; 
    }

    int extractPacket() {

        int saddr_len = sizeof(saddr);
        packet_r = (unsigned char *) malloc(BUFFER_SIZE); // receive data
        memset(packet_r,0,BUFFER_SIZE);
        // receive and copy network packet into the buffer
        ssize_t bufflen = recvfrom( sock_r,
                                    packet_r,
                                    BUFFER_SIZE,
                                    0,
                                    &saddr,
                                    reinterpret_cast<socklen_t *>(&saddr_len));
        if (bufflen < 0)
            return EXIT_FAILURE;
        else {  
            header.extract(packet_r);
            iprot.extract(packet_r, &target);
        }
        
        return static_cast<int>(bufflen);
    }
    
};



int main() {

    RawSocket<65536, ETH_P_ALL> rawsoc_rcv ;



    return 0;
}



