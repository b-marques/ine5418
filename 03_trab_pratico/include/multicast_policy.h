#ifndef MULTICAST_POLICY_H
#define MULTICAST_POLICY_H

#include <queue>
#include <set>
#include "process.h"


namespace distributed_system{

class MulticastMutualExclusionPolicy : private Process
{
  public:
    enum State{        
        RELEASED,
        WANTED,
        HELD
    };
    MulticastMutualExclusionPolicy(int id, int n_process);
    void run();

  private:
    void listen();
    void process_message();
    void send_resource_request();
    void release_resource();
    void wait_resource();
    void process_request();
    void local_clock_increment();
    void local_clock(int time);
    void start_broadcast();
    void lock_mutex();
    void unlock_mutex();

    int             local_clock_;
    State           state_;
    std::set<int>   allow_msg_set_;
    std::queue<int> requisition_queue_;
};

}

#endif //MULTICAST_POLICY_H
