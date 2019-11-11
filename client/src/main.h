
struct client_info
{
    unsigned int id;
    int fd;
};



// MSG TO USER

#define ERR_CLIENT_TIMEOUT "Server connection request timedout ..."

// FCT DECLARATION

#define MAX_RETRY 50

unsigned int id_definer(void);
struct client_info init_connection(unsigned int server_fd);
void hello(int server_fd, unsigned int id_client);

void intHandler(int dummy);
