/**
 *  @file token_ring_policy.h
 *
 *  @brief Header file to the token ring mutual exclusion policy.
 * 
 *  This file contains functions and attributes to simulate the token ring
 *  mutual exclusion policy.
 *
 *  @author Bruno Marques do Nascimento
 *  @date 29/04/2018 
 *  @version 1.0 
 */

#ifndef TOKEN_RING_POLICY_H
#define TOKEN_RING_POLICY_H

#include "process.h"

namespace distributed_system{

/**
 *  Class to simulate the token ring mutual exclusion policy.
 */
class TokenRingMutualExclusionPolicy : private Process
{
  public:
    TokenRingMutualExclusionPolicy(int id, int n_process);
    void  run();

  private:
    void  process_message();
    int   destination();
    void  send_token();
};

}

#endif //TOKEN_RING_POLICY_H