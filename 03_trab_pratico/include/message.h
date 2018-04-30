#ifndef MESSAGE_H
#define MESSAGE_H

#define TOKEN_MSG   0 /* Token message      */
#define REQUEST_MSG 1 /* Resource request   */
#define RELEASE_MSG 2 /* Resource release   */
#define START_MSG   3 /* Start message      */


namespace distributed_system{

typedef struct {
    char source[4];
    char destination[4];
    char type[4];
    char data[100];
} message_t;

}
#endif // MESSAGE_H