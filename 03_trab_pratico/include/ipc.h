/**
 *  @file ipc.h
 *  @author Bruno Marques do Nascimento
 *  @date 29/04/2018 
 *  @version 1.0 
 *
 *  @brief High level approach to the low level IPC API.
 * 
 *  This file is a header file that contains the functions and variables of a 
 *  high level approach to the IPC API.
 *
 */

#ifndef IPC_H
#define IPC_H

#include <netinet/in.h>
#include "message.h"

namespace distributed_system{

/**
 *  @brief Class that supports the user with the IPC API usage.  
 */ 
class IPC
{
  public:
    IPC(int id);
    void       init();
    void       start();
    void       start_listening();
    message_t  receive_msg();
    void       send_msg(message_t);

  private:
    void create_process_fd();
    void configure_process_fd();
                                          
    int                 id_;              //* Process id attached to this IPC.         */
    int                 opt_;             //* Always 1, relative to socket operation.  */
    int                 rcv_process_fd_;  //* File descriptor, to received messages.   */
    int                 send_process_fd_; //* File descriptor, to send messages.       */
    int                 address_length_;  //* Length of the address.                   */
    int                 socket_port_;     //* Socket port that Process will be bind.   */
    struct sockaddr_in  address_;         //* Struct responsible to configure sockets. */
};                                        

}

#endif // IPC_H