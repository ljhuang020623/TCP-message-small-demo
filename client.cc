#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>


#define SERVER_IP "127.0.0.1" 
#define SERVERPORT 8080

int main(){
    // create client socket
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1){
        std::perror("client");
        return EXIT_FAILURE;
    }

    // define the struct sockaddr_in holding the IPv4 address + SERVERPORT + family
    struct sockaddr_in serveraddr;
    std::memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(SERVERPORT);
    // inet_pton convert the IPv4 and IPv6 address from text to binary form
    if (inet_pton(AF_INET, SERVER_IP, &serveraddr.sin_addr) <= 0){
        std::perror("inet_pton");
        close(client_fd);
        return EXIT_FAILURE;
    }

    // connect
    if (connect(client_fd, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr)) < 0){
        std::perror("connect");
        close(client_fd);
        return EXIT_FAILURE;
    }

    std::cout << "The client is connected to the server\n";

    // send message
    const char* msg = "Hello from the client side\n";
    if (write(client_fd, msg, std::strlen(msg)) < 0){
        std::perror("write");
        close(client_fd);
        return EXIT_FAILURE;
    }

    char buffer[1024];
    std::memset(buffer, 0, sizeof(buffer));
    // read till sizeof(buffer) - 1 make sure that the read message doesnt overwrite the null terminator
    // making sure that every message in the buffer is being null-terminated
    ssize_t n = read(client_fd, buffer, sizeof(buffer) - 1);
    if (n < 0){
        std::perror("read");
        close(client_fd);
        return EXIT_FAILURE;
    }

    std::cout << "Server replied: " << buffer << '\n';

    close(client_fd);
    return 0;
}


