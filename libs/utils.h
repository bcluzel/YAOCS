#ifndef _UTILS_H
#define _UTILS_H



#define SERV_PIPE_NAME "/tmp/chat"
#define MAX_USER_NAME_LEN 20
#define CMD_SERVER 0x2
struct user {
    int id;
    char name[MAX_USER_NAME_LEN];
};

struct message {
    int user_id;
    int data_len;
    char *data;
};

#define NUMBER_OF_BYTES_IN_HEADER sizeof(int)*2
/**
 * exit printing error prefixed by `prefix` if `condition` is true (non zero)
 */
void exit_if(int condition, const char *prefix);

int four_char_to_int(char *bytes);
void int_to_four_char(int n, char *bytes);

struct message read_header(int fd);
void recive_mesrsage(int fd, char * data, int data_len);
#endif
