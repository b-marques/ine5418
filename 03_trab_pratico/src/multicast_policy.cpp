#include <iostream>
#include <stdio.h>
#include <string>
#include <random>
#include <unistd.h>

#include "const_data.h"
#include "multicast_policy.h"

using namespace distributed_system;


MulticastMutualExclusionPolicy::MulticastMutualExclusionPolicy(int id,
                                                         int n_process)
    : Process(id, n_process)
{}

void MulticastMutualExclusionPolicy::run()
{
    ipc_.init();
    system("clear");

    PRINT("# --------------------------------- #\n");
    PRINT("# PROCESS_ID: %d                     #\n", id_);
    PRINT("# PORT      : %d                 #\n", SOCKET_BASE_PORT+id_);
    PRINT("# --------------------------------- #\n");    
    PRINT("#                                   #\n");    

    if(id_ != SERVER_ID) {
        ipc_.start_listening();
        received_message_ = ipc_.receive_msg();
        while(1) {
            if (random_computation_time()) {
                client_resource_request();
                
                PRINT("#                                   #\n");    
                PRINT("# Waiting for token...  (listening) #\n");    
                
                received_message_ = ipc_.receive_msg();
                process_message();
            }
            sleep(7);
        }
    /* Process ID == SERVER_ID */
    } else {
        PRINT("#                                   #\n");    
        PRINT("# Waiting for process spawn         #\n");
        PRINT("#                                   #\n");    
        PRINT("# Press ENTER to continue           #\n");
        std::cin.get();
        start_broadcast();
        PRINT("#                                   #\n");    
        PRINT("# Start listening requests...       #\n");    
        ipc_.start_listening();
        while(1) {
            received_message_ = ipc_.receive_msg();
            process_message();
            PRINT("# Waiting for requests...           #\n");    
        }
    }
}

void MulticastMutualExclusionPolicy::process_message()
{
    switch(atoi(received_message_.type)){
        
        case REQUEST_MSG:
            PRINT("#                                   #\n");
            PRINT("# REQUEST FROM PROCESS: %d           #\n", atoi(received_message_.source));
            PRINT("#                                   #\n");
            server_resource_request();
            break;

        case RELEASE_MSG:
            PRINT("#                                   #\n");
            PRINT("# PROCESS %d released resource.      #\n", atoi(received_message_.source));
            PRINT("#                                   #\n");
            server_resource_release();
            break;

        case TOKEN_MSG:
            PRINT("#                                   #\n");
            PRINT("# TOKEN RECEIVED FROM %d             #\n", atoi(received_message_.source));
            PRINT("#                                   #\n");    
            compute();
            client_resource_release();
            break;

        default:
            return;
    }
}

int MulticastMutualExclusionPolicy::destination()
{
    return (id_+1) % n_process_;
}

void MulticastMutualExclusionPolicy::server_resource_request()
{
    if (work_queue.empty()) {
        PRINT("# Work queue is empty.              #\n");    
        
        int process = atoi(received_message_.source);
        work_queue.push(process);

        message_t msg;
        sprintf(msg.destination, "%d", process);
        sprintf(msg.source, "%d", id_);
        sprintf(msg.type, "%d", TOKEN_MSG);

        PRINT("# Sending token to PROCESS %d.       #\n", atoi(msg.destination));    
        ipc_.send_msg(msg);

    } else {
        int process = atoi(received_message_.source);
        PRINT("# Work queue is not empty .         #\n");
        
        PRINT("# Putting PROCESS %d on queue.       #\n", process);           
        work_queue.push(process);
    }
}

void MulticastMutualExclusionPolicy::server_resource_release()
{
    work_queue.pop();

    if (work_queue.empty()) {
        PRINT("# Work queue is empty.              #\n");    
    } else {
        PRINT("# Selecting next process on queue.  #\n");
        int next = work_queue.front();
        PRINT("# PROCESS %d selected.               #\n", next);

        message_t msg;
        sprintf(msg.destination, "%d", next);
        sprintf(msg.source, "%d", id_);
        sprintf(msg.type, "%d", TOKEN_MSG);

        PRINT("# Sending token to PROCESS %d.       #\n", next);    
        ipc_.send_msg(msg);
    }
}

void MulticastMutualExclusionPolicy::client_resource_release()
{
    message_t msg;
    sprintf(msg.destination, "%d", SERVER_ID);
    sprintf(msg.source, "%d", id_);
    sprintf(msg.type, "%d", RELEASE_MSG);

    PRINT("# RELEASING RESOURCE.                #\n");    
    ipc_.send_msg(msg);
}

void MulticastMutualExclusionPolicy::client_resource_request()
{
    message_t msg;
    sprintf(msg.destination, "%d", SERVER_ID);
    sprintf(msg.source, "%d", id_);
    sprintf(msg.type, "%d", REQUEST_MSG);

    PRINT("# REQUESTING RESOURCE.               #\n");    
    ipc_.send_msg(msg);
}

void MulticastMutualExclusionPolicy::start_broadcast()
{
    for (int i = 1; i < n_process_; ++i) {
        message_t msg;
        sprintf(msg.destination, "%d", (id_+i)%n_process_);
        sprintf(msg.source, "%d", id_);
        sprintf(msg.type, "%d", START_MSG);
        ipc_.send_msg(msg);
    } 
}