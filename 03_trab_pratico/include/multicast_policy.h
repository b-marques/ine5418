/**
 *  @file multicast_policy.h
 *
 *  @brief Header file to the multicast with Lamport's clock mutual exclusion 
 *         policy.
 * 
 *  This file contains functions and attributes to simulate the multicast with
 *  Lamport's clock mutual exclusion policy.
 *
 *  @author Bruno Marques do Nascimento
 *  @date 29/04/2018 
 *  @version 1.0 
 */

#ifndef MULTICAST_POLICY_H
#define MULTICAST_POLICY_H

#include <set>
#include "process.h"


namespace distributed_system{

/**
 *  Class to simulate the multicast with Lamport's clock mutual exclusion policy.
 */
class MulticastMutualExclusionPolicy : private Process
{
  public:
    /**
     *  Possibles states of a process.
     */
    enum State{        
        RELEASED,
        WANTED,
        HELD
    };
    MulticastMutualExclusionPolicy(int id, int n_process);
    void run();

  private:
    void listen();
    void wait_resource();
    void start_broadcast();
    void process_message();
    void process_request();
    void release_resource();
    void local_clock(int time);
    void local_clock_increment();
    void send_resource_request();

    volatile State  state_;             //* Actual state of a process  */
    volatile int    local_clock_;       //* Local Lamport's clock      */
    int             request_time_;      //* Clock time of a request    */
    std::set<int>   allow_msg_set_;     //* Received allow message set */
    std::set<int>   requisition_queue_; //* Received requisition   set */
    pthread_mutex_t clock_mutex_;       //* Mutex to clock operation   */
    pthread_mutex_t msg_mutex_;         //* Mutex to message operation */


};

}

#endif //MULTICAST_POLICY_H
