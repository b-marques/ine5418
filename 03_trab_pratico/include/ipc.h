#ifndef IPC_H
#define IPC_H

#include <netinet/in.h>
#include "message.h"

namespace distributed_system{
  
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

    int                 id_;
    int                 opt_;
    int                 output_fd_;
    int                 rcv_process_fd_;
    int                 send_process_fd_;
    int                 address_length_;
    int                 socket_port_;
    struct sockaddr_in  address_;
};

}

#endif // IPC_H