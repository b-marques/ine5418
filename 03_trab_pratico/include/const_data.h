#ifndef GENERAL_INFO_H
#define GENERAL_INFO_H

#define SOCKET_BASE_PORT 65000
#define SERVER_ID        0

#define T1_DEBUG      1
#define T1_DEBUG_MSGS 0

#if T1_DEBUG == 1
    #define PRINT(...) printf(__VA_ARGS__)       
#else
    #define PRINT(...) 
#endif // T1_DEBUG == 0

#if T1_DEBUG_MSGS == 1
    #define PRINT_MSG(...) printf(__VA_ARGS__)       
#else
    #define PRINT_MSG(...) 
#endif // T1_DEBUG_MSGS == 0

#endif // GENERAL_INFO_H