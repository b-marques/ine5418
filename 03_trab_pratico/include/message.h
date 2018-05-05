/**
 *  @file message.h
 *
 *  @brief Constant values and definition of a message.
 * 
 *  This file contains the values of message types and the structure
 *  definition of a message.
 *
 *  @author Bruno Marques do Nascimento
 *  @date 29/04/2018 
 *  @version 1.0 
 */

#ifndef MESSAGE_H
#define MESSAGE_H
/** 
 *  @def TOKEN_MSG
 *  @brief Define the value of token message type.
 */

/** 
 *  @def REQUEST_MSG
 *  @brief Define the value of resource request message type.
 */

/** 
 *  @def RELEASE_MSG
 *  @brief Define the value of resource release message type.
 */

/** 
 *  @def START_MSG
 *  @brief Define the value of start/initialization system message type.
 */
#define TOKEN_MSG   0  //* Token message      */
#define REQUEST_MSG 1  //* Resource request   */
#define RELEASE_MSG 2  //* Resource release   */
#define START_MSG   3  //* Start message      */
#define ALLOW_MSG   4  //* Allow message      */



namespace distributed_system{

/** 
 *  Represents the structure of a message.
 * 	Contains the information about the source, destination, message type
 *  and data of a message.
 */
typedef struct {
    char source[4];      //* Source process id       */
    char destination[4]; //* Destination process id  */
    char type[4];        //* Message type            */
    char data[100];      //* Message data            */
} message_t;

}
#endif // MESSAGE_H