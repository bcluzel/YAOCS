#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include <time.h>

#include "main.h"
#include "utils.h"

int id_client = 0;

int main(int argc, char *argv[])
{

    srand(time(NULL)); // Initiate random sequence

    id_client = id_definer();

    printf("YAOCS launched ! \n");
    int running = 1;
    int fd = open(SERV_PIPE_NAME,O_WRONLY);
    if (fd == -1){
        perror("Pas de serv ");
        exit(EXIT_FAILURE);
        //exec delay retry
    }

    exit_if(init_connection(fd, id_client) == -1, ERR_CLIENT_TIMEOUT);

    while (running)
    {
        /* code */
    }
    
    send_message_server(fd,"BABA \n", id_client);

    close(fd);
    return EXIT_SUCCESS;
}

int init_connection(unsigned int server_fd, unsigned int client_id) {
    hello(server_fd, client_id);

    int r = 0;
    int fildes[2];

    struct message msg;

    do {

        pipe(fildes);
        close(fildes[1]);

        msg = read_header(fildes[0]);
        recive_message(fildes[0], msg.data, msg.data_len);

        ++r;
    } while (r < MAX_RETRY);

    return 0;
}

unsigned int id_definer(void) {
    unsigned int id = rand();
    if (id == 0)
        return id_definer();
    return id;
}

void send_message_server(int server_fd, char *message, unsigned int id_client){
    char buffer[8];
    create_header(buffer, strlen(message)+1, id_client);
    exit_if(write(server_fd,buffer,8) == -1,"hello message write");
    exit_if(write(server_fd,message,strlen(message)+1) == -1,"send serv message write");
}

void hello(int server_fd, unsigned int client_id){
    char buffer[8];
    create_header(buffer, 0, client_id);
    exit_if(write(server_fd, buffer, 8) == -1,"hello write");
}

void create_header(char *buffer, int message_len, unsigned int id_client){
    int_to_four_char(id_client, buffer);
    int_to_four_char(message_len, &buffer[4]); // message len de 0
}