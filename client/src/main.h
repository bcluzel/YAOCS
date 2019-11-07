// MSG TO USER

#define ERR_CLIENT_TIMEOUT "Server connection request timedout ..."

// FCT DECLARATION

#define MAX_RETRY 50

unsigned int id_definer(void);
int init_connection(unsigned int server_fd, unsigned int client_id);
void hello(int server_fd, unsigned int id_client);
void send_message_server(int server_fd, char *message, unsigned int client_id);
void create_header(char *buffer, int message_len, unsigned int id_client);
