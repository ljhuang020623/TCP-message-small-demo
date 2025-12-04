    #include <iostream>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <cstdlib>
    #include <cstdio>
    #include <cstring>
    #include <unistd.h>

    #define SERVERPORT 8080

    int main(){
        // create a tco_socket 
        int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == -1){
            std::perror("server");
            return EXIT_FAILURE;
        }
        // define the struct holding IPv4 address + SERVERPORT + family
        struct sockaddr_in  serveraddr;
        std::memset(&serveraddr, 0, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
        serveraddr.sin_port = htons(SERVERPORT);

        // bind the socket with the struct
        if (bind(server_fd, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr))){
            std::perror("bind");
            return EXIT_FAILURE;
        }

        // set the socket to listening
        if (listen(server_fd, 10)){
            std::perror("listen");
            return EXIT_FAILURE;
        }


        std::cout << "The TCP socket is listening on SERVERPORT: " << SERVERPORT << "\n";

        while (true){
            struct sockaddr_in clientaddr;
            socklen_t client_len = sizeof(clientaddr);

            // accept will return a new fd referring to that socket
            int client_fd = accept(server_fd, reinterpret_cast<sockaddr*>(&clientaddr), &client_len);
            if (client_fd < 0){
                std::perror("accept");
                continue;
            }


            //read message from client
            char buffer[1024];
            memset(buffer, 0, sizeof(buffer));
            ssize_t n = read(client_fd, buffer, sizeof(buffer) - 1);
            if (n < 0){
                std::perror("read");
                close(client_fd);
                continue;
            }

            std::cout << "Received from client: " << buffer << '\n';

            const char* reply = "Hi from the server.\n";
            if (write(client_fd, reply, std::strlen(reply)) < 0){
                std::perror("write");
                close(client_fd);
                continue;
            }

            close(client_fd);
            std::cout << "Closed connection.\n";
        }

        if (close(server_fd)){
            std::perror("close");
            return EXIT_FAILURE;
        }
        return 0;
    }