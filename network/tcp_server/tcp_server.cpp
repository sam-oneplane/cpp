#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <bits/stdc++.h>
#include <signal.h>
#include <atomic>



/*
volatile sig_atomic_t shutdown_sig = 0;
std::atomic<bool> shutdown_req(false); 
static_assert(std::atomic<bool>::is_always_lock_free) 


void signal_handler(int signum) {
    std::cout << "signal ("<< signum <<") received\n";
    shutdown_sig = 1;
}
*/

const char* ip = "0.0.0.0";
constexpr int m_port = 54000;
constexpr int buffsize = 4096;

void error_msg(const std::string&& msg) {
    std::cerr << msg << "\n";

}

typedef struct {
    /* data */
    int clientSocket;
    sockaddr_in client;
    socklen_t clientSize;

} ClientInfo;

typedef struct {
    char host[NI_MAXHOST];      // Client's remote name
    char service[NI_MAXSERV];   // Service (i.e. port) the client is connect on

} MsgInfo;


template <int PORT>
class Socket {

    ClientInfo clientInfo;
    int sock;   // socket file descriptor

public:

    int create_socket() {
        sock =  socket(AF_INET, SOCK_STREAM, 0); // AF_INET : inet4
        return sock;
    }
    int bind_ip_addr() {
        sockaddr_in hint;  /* ipv4 socket addr struct*/
        hint.sin_family = AF_INET;
        hint.sin_port = htons(PORT); /* host to network short (little to big endian)*/
        inet_pton(hint.sin_family, ip, &hint.sin_addr); /* convert IPv4 and IPv6 addresses from text to binary form */
        /* bind to any available ip address */
        return bind(sock, reinterpret_cast<sockaddr*>(&hint), sizeof(hint)); /* bind ip-addr to socket*/
    }


    int get_client_socket() {
        return clientInfo.clientSocket;
    }

    int def_listen() {
        return listen(sock, SOMAXCONN); // 0 success -1 error 
    }

    void connect() {
        MsgInfo msgInfo;

         /* clean buffers*/
        memset(msgInfo.host, 0, NI_MAXHOST);
        memset(msgInfo.service, 0, NI_MAXSERV);

        clientInfo.clientSize = sizeof(clientInfo.client);
        
        clientInfo.clientSocket = accept(sock, 
                                        reinterpret_cast<sockaddr*>(&clientInfo.client),
                                        &clientInfo.clientSize);

        /* address-to-name translation in protocol-independent manner */
        if (getnameinfo(reinterpret_cast<sockaddr*>(&clientInfo.client),
                        clientInfo.clientSize, 
                        msgInfo.host, NI_MAXHOST, 
                        msgInfo.service, 
                        NI_MAXSERV,
                        0) == 0){
            std::cout << msgInfo.host << " connected on port " << msgInfo.service << std::endl;
        }else{
            /* convert IPv4 and IPv6 addresses from binary form to text */
            inet_ntop(AF_INET, &(clientInfo.client.sin_addr), msgInfo.host, NI_MAXHOST);
            /* network to host short conversion*/
            std::cout << msgInfo.host << " connected on port " << ntohs(clientInfo.client.sin_port) << std::endl;
        }
 
        /* Close listening socket */
        close(sock);
    }

    ~Socket() {
        std::cout << "closing server socket!" << std::endl;
        close(clientInfo.clientSocket);
    }

};


template<int BUFF_SIZE>
int echo(int clientSocket) {

    char buff[BUFF_SIZE]; 
    /*clean buffer*/
    memset(buff, 0, BUFF_SIZE);
 
    /* Wait for client to send data */
    int bytesReceived = recv(clientSocket, buff, BUFF_SIZE, 0);
    if (bytesReceived == -1) {
        error_msg("byte recieve error");
        return EXIT_FAILURE;
    }
    if(bytesReceived == 0) {
        error_msg("disconnected error");
        return EXIT_FAILURE;
    }
    std::cout << std::string(buff, 0, bytesReceived) << std::endl;
 
    /* Echo message back to client 
    bytesReceived + 1 :  pad with zero int the end */
    send(clientSocket, buff, bytesReceived + 1, 0);
    return EXIT_SUCCESS;
}




int main() {

    Socket<m_port> mySoc;

    if (mySoc.create_socket() == -1) {
        error_msg("socket creattion error");
        return EXIT_FAILURE;
    }
    if (mySoc.bind_ip_addr() == -1) {
        error_msg("binding socket error");
        return EXIT_FAILURE;
    }
    if (mySoc.def_listen() == -1) {
        error_msg("listening error");
        return EXIT_FAILURE;
    }

    mySoc.connect();

    /*
    {
        struct sigaction action;
        action.sa_handler = signal_handler;
        sigemptyset(&action.sa_mask);
        action.sa_flags = 0;
        sigaction(SIGINT, &action, NULL);
    }
    */
    int clientSocket = mySoc.get_client_socket();
    int status = 0 ;
    while(1) {
        status = echo<buffsize>(clientSocket);
        if (status == EXIT_FAILURE)
            break;
    } 
    std::cout << "end program\n";
    return 0;
}
