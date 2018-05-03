#ifndef TOKEN_RING_POLICY_H
#define TOKEN_RING_POLICY_H

#include "process.h"

namespace distributed_system{

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