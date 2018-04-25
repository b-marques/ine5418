// Client side C/C++ program to demonstrate Socket programming
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <memory>
#include <iostream>

#define PORT 8080
  
int main(int argc, char const *argv[])
{
    std::cout << argc << std::endl;
    int sock_fd = 0;
    struct sockaddr_in serv_addr;
    char hello[1024] = "Hello from client";
    char buffer[1024] = {0};
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    send(sock_fd , static_cast<void*>(&hello) , strlen(hello) , 0 );
    printf("Hello message sent\n");
    read( sock_fd , buffer, 1024);
    printf("%s\n",buffer );
    close(sock_fd);
    return 0;
}