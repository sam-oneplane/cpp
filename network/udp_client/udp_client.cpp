#include <iostream>
#include <string>
#include <atomic>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

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

    void bindsoc() {
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(PORT);
        servaddr.sin_addr.s_addr = INADDR_ANY;
    }

    template<int BUFF_SIZE>
    int sendMsg() {

        std::string userInput;
    
        //		Enter lines of text
        std::cout << "> ";
        getline(std::cin, userInput);

        return  sendto(sockfd,
                       userInput.c_str(),
                       strlen(userInput.c_str()),
                       0,
                       reinterpret_cast<sockaddr*>(&servaddr),
                       sizeof(servaddr));
    }   

    ~Socket() {
        std::cout << "closing client socket\n";
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

    soc.bindsoc();
    int len = 0 ;
    while(true) {
        len = soc.sendMsg<buffsize>();
        std::cout << len << std::endl;
        if (len <= 0)
            break;
    }

    return EXIT_SUCCESS;
}