#include <random>
#include "unistd.h"
#include "const_data.h"
#include "process.h"

using namespace distributed_system;

Process::Process(int id, int n_process)
    : id_{id}, n_process_{n_process}, ipc_{id}
{}

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
int Process::random_computation_time()
{ 
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0,5);
    computation_time_ = dist(rng) + 4;
    return computation_time_;
}