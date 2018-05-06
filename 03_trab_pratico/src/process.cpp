/**
 *  @file process.cpp
 *
 *  @brief Implementation file of Process class.
 * 
 *  This file contains the implementation of useful functions to work with the
 *  spawned process.
 *
 *  @author Bruno Marques do Nascimento
 *  @date 29/04/2018 
 *  @version 1.0 
 */

#include <random>
#include "unistd.h"
#include "const_data.h"
#include "process.h"

using namespace distributed_system;

/**
 *  Constructor where the main attributes are set.
 */
Process::Process(int id, int n_process)
    : id_{id}, n_process_{n_process}, ipc_{id}
{}

/**
 *  Simulate a compute, based on previous calculed time.
 */
void Process::compute()
{
    if (computation_time_) {
        PRINT("#                                   #\n");
        PRINT("# Allocating resource for %d sec(s). #\n", computation_time_);
        PRINT("# _________________________________ #\n");
        for(int i = 0; i < computation_time_; i++) {
            PRINT("# Computing...                      #\n");
            sleep(1);
        }
        PRINT("# Finished computation...__________ #\n");
        return;
    }

    PRINT("# No computation, by-passing token. #\n");
}

/**
 *  Generate a pseudo-random computation time.
 *
 *  @return the pseudo-random computation time generated.
 */
int Process::random_computation_time()
{ 
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0,4);
    computation_time_ = dist(rng);
    computation_time_ = computation_time_ ? 9 : 0;
    return computation_time_;
}