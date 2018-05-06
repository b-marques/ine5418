/**
 *  @file multicast_policy.cpp
 *
 *  @brief Implementation file to the multicast with Lamport's clock mutual 
 *         exclusion policy.
 * 
 *  This file contains functions implementations to simulate multicast with
 *  Lamport's clock mutual exclusion policy.
 *
 *  @author Bruno Marques do Nascimento
 *  @date 29/04/2018 
 *  @version 1.0 
 */

#include <thread>    /* std::thread  */
#include <iostream>  /* std::cin     */
#include <unistd.h>  /* sleep        */
#include <algorithm> /* std::max     */

#include "const_data.h"
#include "multicast_policy.h"

using namespace distributed_system;

/**
 *  Constructor responsible to set main attributes.
 *  Besides set main attributes, it starts the msg_mutex and clock_mutex.
 *
 *  @param id the process id.
 *  @param n_process the total number of process spawned.
 */
MulticastMutualExclusionPolicy::MulticastMutualExclusionPolicy(int id,
                                                               int n_process)
    : Process(id, n_process),
    state_{RELEASED},
    local_clock_{id}
{
    pthread_mutex_init(&clock_mutex_, NULL);
    pthread_mutex_init(&msg_mutex_, NULL);
}

/**
 *  Put the process in a processing loop.
 */
void MulticastMutualExclusionPolicy::run()
{
    ipc_.init();
    system("clear");
    PRINT("# --------------------------------- #\n");
    PRINT("# PROCESS_ID: %d                     #\n", id_);
    PRINT("# PORT      : %d                 #\n", SOCKET_BASE_PORT+id_);
    PRINT("# --------------------------------- #\n");    
    PRINT("#                                   #\n");    

    if (id_ == 0) {
        PRINT("#                                   #\n");    
        PRINT("# Waiting for process spawn         #\n");
        PRINT("#                                   #\n");    
        PRINT("# Press ENTER to continue           #\n");
        std::cin.get();
        start_broadcast();
        ipc_.start_listening();
    } else {
        ipc_.start_listening();
        ipc_.receive_msg();
    }
    
    /* Start listening to other process asyncronously. */
    std::thread thread(&MulticastMutualExclusionPolicy::listen, this);
    
    while (1) {
        switch (state_) {
        case RELEASED:
            PRINT("#                                   #\n");    
            PRINT("# Checking for work...              #\n");    
            pthread_mutex_lock(&clock_mutex_);
            local_clock_increment();
            pthread_mutex_unlock(&clock_mutex_);
            if (random_computation_time()) {
                PRINT("# I Have some work to do...         #\n");    
                PRINT("# Changing state to: WANTED         #\n");
                state_ = WANTED;
            } else {
                PRINT("# I don't have work to do by now... #\n");    
                sleep(7);
            }
            PRINT("#                                   #\n");    
            break;

        case WANTED:
            send_resource_request();
            wait_resource();
            PRINT("#                                   #\n");    
            PRINT("# Changing state to: HELD           #\n");
            state_ = HELD;
            break;

        case HELD:
            compute();
            PRINT("#                                   #\n");    
            PRINT("# Changing state to: RELEASED       #\n");
            state_ = RELEASED;
            release_resource();
            sleep(7);
            break;

        default:
            std::cerr << "Something went wrong, run() switch default case." << std::endl;
            return;
        }
    }
}

/**
 *  Listen to messages from other process.
 *  This method will be started asynchronously.
 */ 
void MulticastMutualExclusionPolicy::listen()
{
    while (1) {
        received_message_ = ipc_.receive_msg();
        process_message();
    }
}

/**
 *  Wait for "ALLOW" message, from all process. 
 */
void MulticastMutualExclusionPolicy::wait_resource()
{   
    allow_msg_set_.clear();
    PRINT("#                                   #\n");
    PRINT("# Waiting resource to be released.  #\n");
    PRINT("#                                   #\n");   
    unsigned int n_allow_msgs = n_process_-1;
    while (allow_msg_set_.size() != n_allow_msgs) {
    }
}

/**
 *  Inform to all spawned process that the system has started. 
 */
void MulticastMutualExclusionPolicy::start_broadcast()
{
    for (int i = 1; i < n_process_; ++i) {
        message_t msg;
        sprintf(msg.destination, "%d", (id_+i)%n_process_);
        sprintf(msg.source, "%d", id_);
        sprintf(msg.data, "%d", local_clock_);
        sprintf(msg.type, "%d", START_MSG);
        ipc_.send_msg(msg);
    } 
}

/**
 *  Handle a received message accordingly to the message type.
 */
void MulticastMutualExclusionPolicy::process_message()
{
    switch(atoi(received_message_.type)) {
    case REQUEST_MSG:
    {
        PRINT("#                                   #\n");
        PRINT("# REQUEST FROM PROCESS: %d           #\n", atoi(received_message_.source));
        process_request();
        break;
    }
    case ALLOW_MSG: 
    {
        int source_id = atoi(received_message_.source);
        PRINT("#                                   #\n");
        PRINT("# PROCESS %d allowed resource use.   #\n", source_id);
        PRINT("#                                   #\n");
        allow_msg_set_.insert(source_id);
        break;
    }
    default:
        return;
    }

    /* Lamport's clock update when receive message */
    int source_clock = atoi(received_message_.data);
    pthread_mutex_lock(&clock_mutex_);
    local_clock(std::max(source_clock, static_cast<int>(local_clock_)) + 1);
    pthread_mutex_unlock(&clock_mutex_);
}

/**
 *  Process request message type. 
 */
void MulticastMutualExclusionPolicy::process_request()
{
    int source_clock = atoi(received_message_.data);
    int source_id    = atoi(received_message_.source);
    
    /* Process already on requisition queue */
    if(requisition_queue_.find(source_id) != requisition_queue_.end())
        return;

    if ((state_ == HELD) ||
        (state_ == WANTED && (request_time_ < source_clock ||
                             (request_time_ == source_clock && id_ < source_id)))) {
        PRINT("# PROCESS %d pushed to req. queue.   #\n", atoi(received_message_.source));
        PRINT("#                                   #\n");
        requisition_queue_.insert(source_id);

    } else {
        PRINT("# Allowing resource to PROCESS %d    #\n", atoi(received_message_.source));
        PRINT("#                                   #\n"); 
        message_t msg;
        sprintf(msg.destination, "%d", source_id);
        sprintf(msg.source, "%d", id_);
        sprintf(msg.data, "%d", local_clock_);
        sprintf(msg.type, "%d", ALLOW_MSG);
        pthread_mutex_lock(&msg_mutex_);
        ipc_.send_msg(msg);
        pthread_mutex_unlock(&msg_mutex_);
    }
}

/**
 *  Inform the releasing of the resource. 
 */
void MulticastMutualExclusionPolicy::release_resource()
{
    PRINT("#                                   #\n");
    PRINT("# Releasing resource...             #\n");
    PRINT("#                                   #\n");
    for (auto queued_id : requisition_queue_) {
        message_t msg;
        sprintf(msg.destination, "%d", queued_id);
        sprintf(msg.source, "%d", id_);
        sprintf(msg.data, "%d", local_clock_);
        sprintf(msg.type, "%d", ALLOW_MSG);
        pthread_mutex_lock(&msg_mutex_);
        ipc_.send_msg(msg);
        pthread_mutex_unlock(&msg_mutex_);
    }
    requisition_queue_.clear();
}

/**
 *  Setter method to local clock.
 *
 *  @param time value that local clock will assume.
 */
void MulticastMutualExclusionPolicy::local_clock(int time)
{
    local_clock_ = time;
}

/**
 *  Increment local clock by one. 
 */
void MulticastMutualExclusionPolicy::local_clock_increment()
{
    ++local_clock_;
}

/**
 *  Inform to all spawned process the will to use the resource. 
 */
void MulticastMutualExclusionPolicy::send_resource_request()
{
    pthread_mutex_lock(&clock_mutex_);
    local_clock_increment();
    request_time_ = local_clock_;
    pthread_mutex_unlock(&clock_mutex_);
    PRINT("#                                   #\n");
    PRINT("# Requesting resource access...     #\n");
    PRINT("#                                   #\n");
    for (int i = 1; i < n_process_; ++i) {
        message_t msg;
        sprintf(msg.destination, "%d", (id_+i)%n_process_);
        sprintf(msg.source, "%d", id_);
        sprintf(msg.data, "%d", request_time_);
        sprintf(msg.type, "%d", REQUEST_MSG);
        pthread_mutex_lock(&msg_mutex_);
        ipc_.send_msg(msg);
        pthread_mutex_unlock(&msg_mutex_);
    }
}
