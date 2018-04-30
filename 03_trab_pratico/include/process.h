#ifndef PROCESS_H
#define PROCESS_H

#include "ipc.h"
#include "message.h"

namespace distributed_system{

class Process
{
  public:
    Process(int id, int n_process);

  protected:
    void compute();
    int  random_computation_time();

    int        id_;
    int        n_process_;
    int        computation_time_;
    IPC        ipc_;
    message_t  received_message_;
};

}
#endif // PROCESS_H