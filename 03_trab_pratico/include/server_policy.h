#ifndef SERVER_POLICY_H
#define SERVER_POLICY_H

#include <queue>
#include "process.h"

namespace distributed_system{

class ServerMutualExclusionPolicy : private Process
{
  public:
    ServerMutualExclusionPolicy(int id, int n_process);
    void  run();

  private:
    void  process_message();
    int   destination();
    void  send_token();
    void  server_resource_request();
    void  server_resource_release();
    void  client_resource_request();
    void  client_resource_release();
    void  start_broadcast();

    std::queue<int> work_queue;
};

}

#endif // SERVER_POLICY_H