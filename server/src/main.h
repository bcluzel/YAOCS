#include "utils.h"
struct user_bank {
    int num_of_users;
    struct user * users;
};

struct message read_header(int fd);
void recive_message(int fd, char * data, int data_len);
void process_hello(struct user_bank *connected_users, unsigned int user_id);
int search_user(struct user_bank *connected_users, unsigned int user_id);
int add_user(struct user_bank *connected_users, unsigned int user_id);
int delete_user(struct user_bank *connected_users, unsigned int user_id);
int change_user_name(struct user_bank *connected_users, unsigned int user_id, char * username);
int open_client_connection(unsigned int user_id);
void broadcast_str(struct user_bank *users, char * message);