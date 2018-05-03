#include <iostream>
#include <stdio.h>
#include <string>
#include <random>
#include <unistd.h>

#include "const_data.h"
#include "token_ring_policy.h"

using namespace distributed_system;

TokenRingMutualExclusionPolicy::TokenRingMutualExclusionPolicy(int id,
                                                               int n_process)
    : Process(id, n_process)
{}

void TokenRingMutualExclusionPolicy::run()
{

    ipc_.init();
    
    /* OUTPUT INFORMATION     */
    system("clear");
    PRINT("# --------------------------------- #\n");
    PRINT("# PROCESS_ID: %d                     #\n", id_);
    PRINT("# PORT      : %d                 #\n", SOCKET_BASE_PORT+id_);
    PRINT("# --------------------------------- #\n");    
    PRINT("#                                   #\n");    

    /* Process ID != 0 */
    if(id_ != SERVER_ID) {
        PRINT("# Waiting for token...  (listening) #\n");    
        ipc_.start_listening();
        while(1) {
            received_message_ = ipc_.receive_msg();
            process_message();
            PRINT("# Waiting for token...  (listening) #\n");    
        }
    /* Process ID == SERVER_ID */
    } else {
        PRINT("#                                   #\n");    
        PRINT("# Waiting for process spawn         #\n");
        PRINT("#                                   #\n");    
        PRINT("# Press ENTER to continue           #\n");
        std::cin.get();
        PRINT("#                                   #\n");    
        PRINT("# Generating token...               #\n");
        send_token();
        PRINT("# Waiting for token...  (listening) #\n");    
        ipc_.start_listening();
        while(1) {
            received_message_ = ipc_.receive_msg();
            process_message();
            PRINT("# Waiting for token...  (listening) #\n");    
        }
    }
}

void TokenRingMutualExclusionPolicy::process_message()
{
    switch(atoi(received_message_.type)){
        case TOKEN_MSG:
            PRINT("#                                   #\n");
            PRINT("# TOKEN RECEIVED                    #\n");
            PRINT("#                                   #\n");    
            random_computation_time();
            compute();
            send_token();
        default:
            return;
    }
}

int TokenRingMutualExclusionPolicy::destination()
{
    return (id_+1) % n_process_;
}



void TokenRingMutualExclusionPolicy::send_token()
{
    PRINT("#                                   #\n");
    PRINT("# Sending token...                  #\n");    
    message_t msg;
    sprintf(msg.source, "%d", id_);
    sprintf(msg.destination, "%d", destination());
    sprintf(msg.type, "%d", TOKEN_MSG);

    ipc_.send_msg(msg);
}