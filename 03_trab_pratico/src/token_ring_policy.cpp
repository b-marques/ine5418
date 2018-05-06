/**
 *  @file token_ring_policy.cpp
 *
 *  @brief Implementation file to the token ring mutual exclusion policy.
 * 
 *  This file contains functions implementations to simulate the token ring
 *  mutual exclusion policy.
 *
 *  @author Bruno Marques do Nascimento
 *  @date 29/04/2018 
 *  @version 1.0 
 */

#include <iostream> /* std::cin */
#include <unistd.h> /* sleep    */

#include "const_data.h"
#include "token_ring_policy.h"

using namespace distributed_system;

/**
 *  Constructor responsible to set main attributes.
 *
 *  @param id the process id.
 *  @param n_process the total number of process spawned.
 */
TokenRingMutualExclusionPolicy::TokenRingMutualExclusionPolicy(int id,
                                                               int n_process)
    : Process(id, n_process)
{}

/**
 *  Put the process in a processing loop accordingly to his id.
 */
void TokenRingMutualExclusionPolicy::run()
{
    ipc_.init();
    system("clear");
    PRINT("# --------------------------------- #\n");
    PRINT("# PROCESS_ID: %d                     #\n", id_);
    PRINT("# PORT      : %d                 #\n", SOCKET_BASE_PORT+id_);
    PRINT("# --------------------------------- #\n");    
    PRINT("#                                   #\n");    

    /* Process ID != 0 */
    if(id_ != SERVER_ID) {
        PRINT("#                                   #\n");
        PRINT("# Waiting for token...  (listening) #\n");    
        ipc_.start_listening();
        while(1) {
            received_message_ = ipc_.receive_msg();
            process_message();
            PRINT("#                                   #\n");
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
        PRINT("#                                   #\n");
        PRINT("# Waiting for token...  (listening) #\n");    
        ipc_.start_listening();
        while(1) {
            received_message_ = ipc_.receive_msg();
            process_message();
            PRINT("# Waiting for token...  (listening) #\n");    
        }
    }
}

/**
 *  Handle a received message accordingly to the message type.
 */
void TokenRingMutualExclusionPolicy::process_message()
{
    switch(atoi(received_message_.type)){
    case TOKEN_MSG:
        system("clear");    
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

/**
 *  Calculate que destination of a message accordingly to this policy.
 *
 *  @return the destination process id.
 */
int TokenRingMutualExclusionPolicy::destination()
{
    return (id_+1) % n_process_;
}

/**
 *  Build and send a token message to the next process on the ring. 
 */
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
