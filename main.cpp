#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

#include "httpRequest.hpp"
#include "httpResponse.hpp"

#define PORT 56
#define BUFFER_SIZE 1024

int socketclose;

void    sig_handler(int signal) {
    (void)signal;
    std::cout << "SIGNAL SIGINT" << std::endl;
    close(socketclose);
    exit(0);
}


int main() {
    char buffer[BUFFER_SIZE];
    char resp[] = "HTTP/1.1 200 OK\r\n"
                  "Server: webserver-c\r\n"
                  "Content-type: text/html\r\n\r\n"
                  "<html>hello, world</html>\r\n";

    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    socketclose = sockfd;
    if (sockfd == -1) {
        perror("webserver (socket)");
        return 1;
    }
    // fcntl(sockfd, F_SETFL, O_NONBLOCK);
    // int value = 1;
    // if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)) == -1)
    // {
    //     perror("setsockopt :");
    //     exit(-1);
    // }

    // Create the address to bind the socket to
    struct sockaddr_in host_addr;
    int host_addrlen = sizeof(host_addr);

    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(PORT);
    host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Create client address
    struct sockaddr_in client_addr;
    int client_addrlen = sizeof(client_addr);

    if (bind(sockfd, (struct sockaddr *)&host_addr, host_addrlen) != 0) {
        perror("webserver (bind)");
        return 1;
    }
    if (listen(sockfd, SOMAXCONN) != 0) {
        perror("webserver (listen)");
        return 1;
    }
    signal(SIGINT, sig_handler);
    std::cout << "server open" << std::endl;
    for (;;) {
        int newsockfd = accept(sockfd, (struct sockaddr *)&host_addr,
                               (socklen_t *)&host_addrlen);
        if (newsockfd < 0) {
            perror("webserver (accept)");
            continue;
        }
        printf("connection accepted\n");

        int sockn = getsockname(newsockfd, (struct sockaddr *)&client_addr,
                                (socklen_t *)&client_addrlen);
        if (sockn < 0) {
            perror("webserver (getsockname)");
            continue;
        }

        int valread = read(newsockfd, buffer, BUFFER_SIZE);
        if (valread < 0) {
            perror("webserver (read)");
            continue;
        }


        std::string sBuff(buffer);
        HttpRequest request;
        request.readRequest(sBuff);

        HttpResponse httpRes;
        httpRes.response(request, newsockfd);

        char method[BUFFER_SIZE], uri[BUFFER_SIZE], version[BUFFER_SIZE];
        sscanf(buffer, "%s %s %s", method, uri, version);
        printf("[%s:%u] %s %s %s\n", inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port), method, version, uri);
        close(newsockfd);
    }

    return 0;
}