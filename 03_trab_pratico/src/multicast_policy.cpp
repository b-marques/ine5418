#include <sys/file.h>
#include <string>
#include <thread>    /* std::thread  */
#include <stdio.h>   /* printf       */
#include <fstream>   /* std::fstream */
#include <iostream>  /* std::cin     */
#include <unistd.h> 
#include <algorithm> /* std::max     */

#include "const_data.h"
#include "multicast_policy.h"

using namespace distributed_system;

MulticastMutualExclusionPolicy::MulticastMutualExclusionPolicy(int id,
                                                         int n_process)
    : Process(id, n_process),
      local_clock_{id},
      state_{RELEASED}
{
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
            PRINT("#                                   #\n");
            local_clock_increment();
            if (random_computation_time()) {
                PRINT("#                                   #\n");    
                PRINT("# I Have some work to do...         #\n");    
                PRINT("# Changing state to: WANTED         #\n");
                PRINT("#                                   #\n");    
                state_ = WANTED;
            } else {
                sleep(4);
            }
            break;

        case WANTED:
            send_resource_request();
            wait_resource();
            state_ = HELD;
            break;

        case HELD:
            compute();
            state_ = RELEASED;
            release_resource();
            sleep(3);

            break;

        default:
            std::cerr << "Something went wrong, run() switch default case." << std::endl;
            return;
        }
    }
}

/**
 *  Listen to messages from other process.
 */ 
void MulticastMutualExclusionPolicy::listen()
{
    while (1) {
        received_message_ = ipc_.receive_msg();
        process_message();
        sleep(1);
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
        PRINT("#                                   #\n");
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

    /* Lamport clock update when receive message */
    int source_clock = atoi(received_message_.data);
    local_clock(std::max(source_clock, local_clock_) + 1);
}
/**
 *  Inform to all spawned process the will to use the resource. 
 */
void MulticastMutualExclusionPolicy::send_resource_request()
{
    local_clock_increment();
    PRINT("#                                   #\n");
    PRINT("# Requesting resource access...     #\n");
    PRINT("#                                   #\n");
    for (int i = 1; i < n_process_; ++i) {
        message_t msg;
        sprintf(msg.destination, "%d", (id_+i)%n_process_);
        sprintf(msg.source, "%d", id_);
        sprintf(msg.data, "%d", local_clock_);
        sprintf(msg.type, "%d", REQUEST_MSG);
        ipc_.send_msg(msg);
    }
}

/**
 *  Inform the releasing of the resource. 
 */
void MulticastMutualExclusionPolicy::release_resource()
{
    local_clock_increment();
    PRINT("#                                   #\n");
    PRINT("# Releasing resource...             #\n");
    PRINT("#                                   #\n");
    while (!requisition_queue_.empty()) {
        message_t msg;
        sprintf(msg.destination, "%d", requisition_queue_.front());
        sprintf(msg.source, "%d", id_);
        sprintf(msg.data, "%d", local_clock_);
        sprintf(msg.type, "%d", ALLOW_MSG);
        ipc_.send_msg(msg);
        requisition_queue_.pop();
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
        sleep(0.5);
    }
}

/**
 *  Process request message type. 
 */
void MulticastMutualExclusionPolicy::process_request()
{
    int source_clock = atoi(received_message_.data);
    int source_id    = atoi(received_message_.source);
    if (state_ == HELD){
        PRINT("#                                   #\n");
        PRINT("# PROCESS %d pushed to req. queue.   #\n", atoi(received_message_.source));
        PRINT("#                                   #\n");
        requisition_queue_.push(source_id);
    } else if (state_ == WANTED &&
        (local_clock_ < source_clock || (local_clock_ == source_clock && id_ < source_id))) {
        PRINT("#                                   #\n");
        PRINT("# PROCESS %d pushed to req. queue.   #\n", atoi(received_message_.source));
        PRINT("#                                   #\n");
        requisition_queue_.push(source_id);

    } else {
        PRINT("#                                   #\n");
        PRINT("# Allowing resource to PROCESS %d    #\n", atoi(received_message_.source));
        PRINT("#                                   #\n"); 
        message_t msg;
        sprintf(msg.destination, "%d", source_id);
        sprintf(msg.source, "%d", id_);
        sprintf(msg.data, "%d", local_clock_);
        sprintf(msg.type, "%d", ALLOW_MSG);
        ipc_.send_msg(msg);
    }
}

/**
 *  Increment local clock by one. 
 */
void MulticastMutualExclusionPolicy::local_clock_increment()
{
    ++local_clock_;
}

/**
 *  Setter method to local clock. 
 */
void MulticastMutualExclusionPolicy::local_clock(int time)
{
    local_clock_ = time;
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
 *  Wait until mutex file lock succeed. 
 */
void MulticastMutualExclusionPolicy::lock_mutex()
{
    PRINT("#                                   #\n");
    PRINT("# Locking file to make a request.   #\n");
    PRINT("#                                   #\n");   
    while(open(LOCK_FILE, O_CREAT | O_EXCL) == -1);
    PRINT("#                                   #\n");
    PRINT("# File locked successfully,         #\n");
    PRINT("#                                   #\n");  
}

/**
 *  Mutex file unlock. 
 */
void MulticastMutualExclusionPolicy::unlock_mutex()
{
    PRINT("#                                   #\n");
    PRINT("# Unlocking mutex file.             #\n");
    PRINT("#                                   #\n");   
    if(remove(LOCK_FILE))
        std::cerr << "Error deleting file..." << std::endl;

}