/**
 *  @file process.h
 *
 *  @brief Class representation of a process.
 * 
 *  This file contains the representation of a process, with the necessary
 *  functions and attributes to represent it, to later communicate with another
 *  one.
 *
 *  @author Bruno Marques do Nascimento
 *  @date 29/04/2018 
 *  @version 1.0 
 */

#ifndef PROCESS_H
#define PROCESS_H

#include "ipc.h"
#include "message.h"

namespace distributed_system{

/**
 *  Class with the core attributes of a simple process in the work context.  
 */ 
class Process
{
  public:
    Process(int id, int n_process);

  protected:
    void compute();
    int  random_computation_time();

    int        id_;               //* Process ID                    */
    int        n_process_;        //* Count of all spawned process  */
    int        computation_time_; //* Next compute duration         */ 
    IPC        ipc_;              //* IPC attached to this process  */
    message_t  received_message_; //* Last received message         */
};

}
#endif //PROCESS_H