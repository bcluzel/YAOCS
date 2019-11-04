#ifndef _UTILS_H
#define _UTILS_H



#define SERV_PIPE_NAME "/tmp/chat"
#define MAX_USER_NAME_LEN 20
#define MAX_MESSAGE_LEN 20
struct user {
    int id;
    char name[MAX_USER_NAME_LEN];
};

struct message{
    int user_id;
    int data_len;
    char data[MAX_MESSAGE_LEN];
};

#define NUMBER_OF_BYTES_IN_HEADER sizeof(int)*2
/**
 * exit printing error prefixed by `prefix` if `condition` is true (non zero)
 */
void exit_if(int condition, const char *prefix);

#endif
