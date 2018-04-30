#include <memory>   
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <iostream>

#include "ipc.h"
#include "const_data.h"

using namespace distributed_system;

IPC::IPC(int id) :
    id_{id}, opt_{1}, address_length_{sizeof(address_)}
{}

void IPC::init()
{
    create_process_fd();
    configure_process_fd();
}

void IPC::create_process_fd()
{
    // Creating process socket file descriptor
    if ((rcv_process_fd_ = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cerr << "IPC: socket file descriptor creation failed." << std::endl;
        exit(EXIT_FAILURE);
    }
}

void IPC::configure_process_fd()
{
    /* Configuring process socket file descriptor */
    if (setsockopt(rcv_process_fd_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                   &opt_, sizeof(opt_)))
    {
        std::cerr << "IPC: setsockopt operation failed." << std::endl;
        exit(EXIT_FAILURE);
    }

    address_.sin_family = AF_INET;                               /* Setting to IPV4            */
    address_.sin_addr.s_addr = INADDR_ANY;                       /* Receiving from any address */
    address_.sin_port = htons( SOCKET_BASE_PORT + id_ ); /* Socket port to bind        */
      
    /* Attaching socket to the port */
    if (bind(rcv_process_fd_, (struct sockaddr *)&address_, 
                                   sizeof(address_))<0)
    {
        std::cerr << "IPC: bind operation failed." << std::endl;;
        exit(EXIT_FAILURE);
    }
}

void IPC::start_listening()
{
    if (listen(rcv_process_fd_, 10) < 0)
    {
        std::cerr << "IPC: listen operation failed" << std::endl;
        exit(EXIT_FAILURE);
    }
}

message_t IPC::receive_msg()
{
    message_t msg;        /* Message received data */
    int       new_socket; /* New connection socket */

    if ((new_socket = accept(rcv_process_fd_, (struct sockaddr *)&address_, 
                       (socklen_t*)&address_length_))<0)
    {
        std::cerr << "IPC: accept operation failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    read(new_socket , &msg, sizeof(message_t));
    close(new_socket);

    PRINT_MSG("======================\n");
    PRINT_MSG("| MESSAGE RECEIVED   |\n");
    PRINT_MSG("| MSG      SOURCE: %d |\n", atoi(msg.source));
    PRINT_MSG("| MSG DESTINATION: %d |\n", atoi(msg.destination));
    PRINT_MSG("| MSG    MSG TYPE: %d |\n", atoi(msg.type));
    PRINT_MSG("======================\n");
    return msg;
}

void IPC::send_msg(message_t msg)
{
    send_process_fd_ = 0;
    struct sockaddr_in serv_addr;

    // Creating process socket file descriptor
    if ((send_process_fd_ = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cerr << "IPC: socket file descriptor creation failed." << std::endl;
        exit(EXIT_FAILURE);
    }

    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SOCKET_BASE_PORT + atoi(msg.destination));

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        std::cerr << "IPC: Invalid address/ Address not supported" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (connect(send_process_fd_, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << "IPC: send_msg() -> connection failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    write(send_process_fd_, static_cast<void*>(&msg) , sizeof(msg));

    PRINT_MSG("======================\n");
    PRINT_MSG("| MESSAGE SEND       |\n");
    PRINT_MSG("| MSG      SOURCE: %d |\n", atoi(msg.source));
    PRINT_MSG("| MSG DESTINATION: %d |\n", atoi(msg.destination));
    PRINT_MSG("| MSG    MSG TYPE: %d |\n", atoi(msg.type));
    PRINT_MSG("======================\n");
}