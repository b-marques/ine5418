#include <string>
#include <iostream>
#include "const_data.h"
#include "server_policy.h"
#include "multicast_policy.h"
#include "token_ring_policy.h"

int main(int argc, char const *argv[])
{
    /* Arguments extraction */
    int id = std::stoi(argv[1]);        /* Process ID              */
    int n_process = std::stoi(argv[2]); /* Total number of process */
    std::string policy(argv[3]);        /* Mutual exclusion policy */

    
    if(policy == "--token_ring") {
        distributed_system::TokenRingMutualExclusionPolicy process(id, n_process);
        process.run();
    }

    if(policy == "--server") {
        distributed_system::ServerMutualExclusionPolicy    process(id, n_process);
        process.run();
    }

    if(policy == "--multicast") {
        distributed_system::MulticastMutualExclusionPolicy process(id, n_process);
        process.run();
    }
}
