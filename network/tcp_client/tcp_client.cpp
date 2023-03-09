#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <atomic>
#include <signal.h>


//volatile sig_atomic_t shutdown_sig = 0;

const char* ip = "127.0.0.1";


void atexit_handler() {
    std::cout << "exit the client!!!\n";
}
/*
void signal_handler(int signum) {
    std::cout << "signal ("<< signum <<") received\n";
    shutdown_sig = 1;
}
*/
void error_msg(const std::string&& msg) {
    std::cerr << msg << "\n";

}

typedef struct {
    /* data */
    int sock;  // socket descriptor
    int connectRes;
    sockaddr_in hint;

} SocketInfo;

template<int PORT>
class Client {

    SocketInfo socketInfo;

public:

    int create_socket() {
        socketInfo.sock =  socket(AF_INET, SOCK_STREAM, 0); // AF_INET : inet4
        return socketInfo.sock;
    }

    int connect_soc() {
        socketInfo.hint.sin_family = AF_INET;
        socketInfo.hint.sin_port = htons(PORT);
        inet_pton(AF_INET, ip, &socketInfo.hint.sin_addr);
        socketInfo.connectRes = connect(socketInfo.sock, 
                                        reinterpret_cast<sockaddr*>(&socketInfo.hint), 
                                        sizeof(socketInfo.hint));
        return socketInfo.connectRes;
    }

    ~Client() {
        std::cout << "closing client socket!!!\n";
        close(socketInfo.sock);
    }
};

template<int BUFF_SIZE>
int sendMsg(int sock) {

    char buff[BUFF_SIZE];
    std::string userInput;
    
    //		Enter lines of text
    std::cout << "> ";
    getline(std::cin, userInput);

    //		Send to server
    if (send(sock, userInput.c_str(), userInput.size() + 1, 0) == -1){
        std::cout << "Could not send to server! Whoops!\r\n";
        return EXIT_SUCCESS;
    }

    //		Wait for response
    memset(buff, 0, BUFF_SIZE); // clear buff
    int bytesReceived = recv(sock, buff, 4096, 0);
    if (bytesReceived == -1){
        std::cout << "There was an error getting response from server\r\n";
        return EXIT_FAILURE;
    }else{
        //		Display response
        std::cout << "SERVER> " << std::string(buff, bytesReceived) << "\r\n";
    }
    return EXIT_SUCCESS;

}


int main() {

    std::atexit(atexit_handler);

    Client<54000> client;

    int sock = client.create_socket();
    if (sock == -1) {
        error_msg("socket creattion error");
        return EXIT_FAILURE;
    }

    if (client.connect_soc() == -1) {
        error_msg("socket connect error");
        return EXIT_FAILURE;
    }

    do {
        sendMsg<4096>(sock); 
    }while(1);

    return 0;
}
