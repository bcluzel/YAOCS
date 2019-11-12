#ifndef _UTILS_H
#define _UTILS_H

#define ID_SERVER 0

#define SERV_PIPE_NAME "/tmp/chat"
#define PIPE_DEFAULT_FOLDER "/tmp/"
#define PIPE_FOLDER_PLUS_INTREP_LEN 18
#define MAX_USER_NAME_LEN 20
#define MAX_MESSAGE_LEN 254
#define CMD_SERVER 0x2
#define CMD_USER '/'
#define END_OF_CONNECTION  0x1

#define DEFAULT_NAME "Bobby"

#define FILE_DESCRIPTOR_RECIVED 0xA0
struct user {
    unsigned int id;
    char name[MAX_USER_NAME_LEN];
    int fd;
};

struct message {
    int user_id;
    int data_len;
    char *data;
};

#define NUMBER_OF_BYTES_IN_HEADER 8
/**
 * exit printing error prefixed by `prefix` if `condition` is true (non zero)
 */
void exit_if(int condition, const char *prefix);

unsigned int four_char_to_int(char *bytes);
void int_to_four_char(unsigned int n, char *bytes);

struct message read_header(int fd);
int read_header_nb(int fd, struct message *answer);
void recive_message(int fd, char * data, int data_len);

void send_message_str(int server_fd, char *message, unsigned int client_id);
void create_header(char *buffer, int message_len, unsigned int id_client);
void send_message(int server_fd, char *message, unsigned int id_client, int message_len);
char * path_mkfifo_client(unsigned int id_client, char *buffer);
int read_stdin(char *buffer);
#endif
