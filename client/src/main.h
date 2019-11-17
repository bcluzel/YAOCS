
struct client_info
{
    unsigned int id;
    int fd;
    int connected;
};



// MSG TO USER

#define ERR_CLIENT_TIMEOUT "Server connection request timedout ..."

// FCT DECLARATION

#define MAX_RETRY 1

unsigned int id_definer(void);
struct client_info init_connection(unsigned int server_fd);
void hello(int server_fd, unsigned int id_client);
void end_of_connection();
void send_message_str_server(int server_fd, char *message, unsigned int client_id);
void send_message_server(int server_fd, char *message, unsigned int client_id, int message_len);

void lock(void);
void unlock(void);

void intHandler(int dummy);
void cleanExit(int dummy);
