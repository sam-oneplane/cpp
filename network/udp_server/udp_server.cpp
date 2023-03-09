#include <iostream>
#include <string>
#include <atomic>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

const char* ip = "0.0.0.0";
constexpr int port_n = 53002;
constexpr int buffsize = 4096;


void error_msg(const std::string&& msg) {
    std::cerr << msg << "\n";
}


template <int PORT>
class Socket {
    int sockfd;   // socket file descriptor
    sockaddr_in servaddr = {0};

public:

    int create_socket() {
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        return sockfd;
    }

    int bindsoc() {
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(PORT);
        servaddr.sin_addr.s_addr = INADDR_ANY;
        /*
        inet_pton(servaddr.sin_family,
                 ip, 
                 &servaddr.sin_addr); 
        */
        /* bind to any available ip address */
        return bind(sockfd, 
                    reinterpret_cast<sockaddr*>(&servaddr), 
                    sizeof(servaddr)); /* bind ip-addr to socket*/
    }

    template<int BUFF_SIZE>
    int get() {
        char buff[BUFF_SIZE] = {0}; 
        socklen_t soclen = 0;
        int bytesReceived = recvfrom(sockfd,
                                        reinterpret_cast<char*>(buff),
                                        BUFF_SIZE,
                                        MSG_WAITALL,
                                        0,
                                        &soclen);
        std::cout << std::string(buff, 0, bytesReceived) << std::endl;
        return EXIT_SUCCESS;
    }

    ~Socket() {
        std::cout << "closing server socket\n";
        close(sockfd);
    }

};


int main() {

    Socket<port_n> soc;
    int fd = soc.create_socket();

    if(fd == -1){
        error_msg("socket creattion error");
        return EXIT_FAILURE;
    }
    if(soc.bindsoc() == -1) {
        error_msg("bind socket failed");
        return EXIT_FAILURE;
    }

    int thd = 10;
    while (thd > 0) {
        soc.get<buffsize>() ;
        thd--;
    }
    return EXIT_SUCCESS;
}