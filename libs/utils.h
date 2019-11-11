#ifndef _UTILS_H
#define _UTILS_H



#define SERV_PIPE_NAME "/tmp/chat"
#define PIPE_DEFAULT_FOLDER "/tmp/"
#define MAX_USER_NAME_LEN 20

#define CMD_SERVER 0x2
#define CMD_USER '/'
#define FILE_DESCRIPTOR_TX 0x1
#define END_OF_CONNECTION  0x2



#define FILE_DESCRIPTOR_RECIVED 0xA0
struct user {
    int id;
    char name[MAX_USER_NAME_LEN];
    int writing_filedes;
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

unsigned int four_char_to_int(char *bytes);
void int_to_four_char(unsigned int n, char *bytes);

struct message read_header(int fd);
void recive_message(int fd, char * data, int data_len);

void send_message_server(int server_fd, char *message, unsigned int client_id);
void create_header(char *buffer, int message_len, unsigned int id_client);
void send_message(int server_fd, char *message, unsigned int id_client, int message_len);


#endif
