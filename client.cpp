#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;
    int sock = 0; long valread;
    struct sockaddr_in serv_addr;
    std::string hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "\n Socket creation error" << std::endl;
        return -1;
    }
    
    memset(&serv_addr, '0', sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        std::cout << "\nInvalid address/ Address not supported" << std::endl; 
        return -1;
    }
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cout << "\nConnection Failed" << std::endl;
        return -1;
    }
    send(sock , hello.c_str() , hello.length() , 0 );
    std::cout << "Hello message sent" << std::endl;
    valread = read( sock , buffer, 1024);
    std::cout << buffer << std::endl;
    return 0;
}
