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
void broadcast_str_excluding(struct user_bank *users, char * message, int excluded_id);
void intHandler(int dummy);
void end_of_connection();
void process_list_users(struct user_bank *connected_users, struct message message);
void process_change_name(struct user_bank *connected_users, struct message message);
void process_msg(struct user_bank *connected_users, struct message message);
void process_help(struct user_bank *connected_users, struct message message);