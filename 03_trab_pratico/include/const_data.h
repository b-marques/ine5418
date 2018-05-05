/**
 *  @file const_data.h
 *
 *  @brief Constant default values used by the process and the system.
 * 
 *  This file contains some default values such as debug flags, default socket
 *  base port, printing macros, etc.
 *
 *  @author Bruno Marques do Nascimento
 *  @date 29/04/2018 
 *  @version 1.0 
 */

#ifndef GENERAL_INFO_H
#define GENERAL_INFO_H

/** 
 *  @def SOCKET_BASE_PORT
 *  @brief Default socket base port.
 *
 *  Initial base port value utilized by the process. The process will bind to the
 *  port number resulting from the sum of SOCKET_BASE_PORT and his ID. 
 */

/** 
 *  @def SERVER_ID
 *  @brief Define the process id corresponding to server.
 */
#define SOCKET_BASE_PORT 65000
#define SERVER_ID        0

/** 
 *  @def T1_DEBUG
 *  @brief Enable/disable process workflow printing.
 *
 *  When is set to 1 will print process workflow, when 0 will print nothing.
 */

/** 
 *  @def T1_DEBUG_MSGS
 *  @brief Enable/disable process exchanged messages printing.
 *
 *  When is set to 1 will print enxchanged messages, when 0 will print nothing.
 */
#define T1_DEBUG      1
#define T1_DEBUG_MSGS 0


/** 
 *  @def PRINT(...)
 *  @brief Macro to print process workflow.
 *
 *  This macro prints the process workflow when the variable T1_DEBUG is set to 1.
 *  If T1_DEBUG is 0, this macro will be ignored.
 */
#if T1_DEBUG == 1
    #define PRINT(...) printf(__VA_ARGS__)       
#else
    #define PRINT(...) 
#endif //T1_DEBUG == 1

/** 
 *  @def PRINT_MSG(...)
 *  @brief Macro to print messages exchanged between process.
 *
 *  This macro prints the messages exchanged between the spawned process, showing
 *  source, destination and message type.
 */
#if T1_DEBUG_MSGS == 1
    #define PRINT_MSG(...) printf(__VA_ARGS__)       
#else
    #define PRINT_MSG(...) 
#endif //T1_DEBUG_MSGS == 1


#endif //GENERAL_INFO_H