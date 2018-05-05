/**
 *  @file server_policy.h
 *
 *  @brief Header file to the server/coordinator mutual exclusion policy.
 * 
 *  This file contains functions and attributes to simulate the server/coordinator
 *  mutual exclusion policy.
 *
 *  @author Bruno Marques do Nascimento
 *  @date 29/04/2018 
 *  @version 1.0 
 */

#ifndef SERVER_POLICY_H
#define SERVER_POLICY_H

#include <queue>
#include "process.h"

namespace distributed_system{
    
/**
 *  Class to simulate the server/coordinator mutual exclusion policy.
 */
class ServerMutualExclusionPolicy : private Process
{
  public:
    ServerMutualExclusionPolicy(int id, int n_process);
    void  run();

  private:
    void  process_message();
    int   destination();
    void  server_resource_request();
    void  server_resource_release();
    void  client_resource_request();
    void  client_resource_release();
    void  start_broadcast();

    std::queue<int> work_queue_; //* Work queue of process */
};

}

#endif //SERVER_POLICY_H