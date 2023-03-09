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




struct IFreq {
    struct ifreq ifreq_i, ifreq_c, ifreq_ip;

    int extract_index(int rawsoc) {
        memset(&ifreq_i, 0, sizeof(ifreq_i));
        strncpy(ifreq_i.ifr_name, "wlan0",  IFNAMSIZ-1); // get interface name
        return ioctl(rawsoc, SIOCGIFINDEX, &ifreq_i);
    }

    int extract_mac() {
        
    }


};