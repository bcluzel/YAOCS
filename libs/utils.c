#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

#include "utils.h"

void exit_if(int condition, const char *prefix)
{
    if (condition) {
        perror(prefix);
        exit(1);
    }
}

void int_to_four_char(unsigned int n, char *bytes){
    bytes[0] = (n >> 24) & 0xFF;
    bytes[1] = (n >> 16) & 0xFF;
    bytes[2] = (n >> 8) & 0xFF;
    bytes[3] = n & 0xFF;
}

unsigned int four_char_to_int(char *bytes){
    unsigned int n;
    n = bytes[0] << 24 | bytes[1] << 16 | bytes[2] << 8 | bytes[3] << 0;
    return n;
}

struct message read_header(int fd){

    static char buffer [NUMBER_OF_BYTES_IN_HEADER];
    static int head=0;
    while (head<NUMBER_OF_BYTES_IN_HEADER)
    {
        int answer_read = read(fd,&buffer[head],1);
        exit_if(answer_read == -1, "read read_header");
        head += answer_read;
    }
    head = 0;
    struct message answer;
    answer.user_id = four_char_to_int(&buffer[0]);
    answer.data_len = four_char_to_int(&buffer[4]);
    return answer;
}

int read_header_nb(int fd, struct message *answer){

    static char buffer [NUMBER_OF_BYTES_IN_HEADER];
    static int head=0;
    int header_readed = 0;
    int answer_read = read(fd,&buffer[head],1);

    if (answer_read == -1)
    {
        if (errno == EAGAIN)
        {
            usleep(500);
            return header_readed;
        }else
        {
            perror("read read_header");
            exit(1);
        }   
    }
    
    head += answer_read;
    if (head >= 8)
    {
        head = 0;
        answer->user_id = four_char_to_int(&buffer[0]);
        answer->data_len = four_char_to_int(&buffer[4]);
        header_readed = 1;
    }else
    {
        header_readed = 0;
    }
    
    
    return header_readed;
}

void recive_message(int fd, char * data, int data_len){
    int head=0;
    while (head<data_len)
    {
    int answer_read = read(fd,&data[head],1);
    exit_if(answer_read == -1, "read read_client");
    head += answer_read;
    }
    head = 0;
}


void send_message_str(int server_fd, char *message, unsigned int id_client){
    char buffer[8];
    create_header(buffer, strlen(message)+1, id_client);
    exit_if(write(server_fd,buffer,8) == -1,"hello message write");
    exit_if(write(server_fd,message,strlen(message)+1) == -1,"send serv message write");
}

void send_message(int server_fd, char *message, unsigned int id_client, int message_len){
    char buffer[8];
    create_header(buffer, message_len, id_client);
    exit_if(write(server_fd,buffer,8) == -1,"hello message write");
    exit_if(write(server_fd,message,message_len) == -1,"send serv message write");
}

void create_header(char *buffer, int message_len, unsigned int id_client){
    int_to_four_char(id_client, buffer);
    int_to_four_char(message_len, &buffer[4]); // message len de 0
}


char * path_mkfifo_client(unsigned int id_client, char *buffer){
    sprintf(buffer,"%s%u",PIPE_DEFAULT_FOLDER,id_client);
    return buffer;
}

int read_stdin(char *buffer){

    static int head=0;
    int header_readed=0;
    int answer_read = read(STDIN_FILENO,&buffer[head],1);
    if (answer_read == -1)
    {
        if (errno == EAGAIN)
        {
            usleep(500);
            return header_readed;
        }else
        {
            perror("read read_header");
            exit(1);
        }   
    }else
    {
        if (head >= MAX_MESSAGE_LEN || buffer[head] == 10)
        {
            buffer[head+1] = '\0';
            head = 0;
            answer_read = 0;
            header_readed = 1;
        }else
        {
            header_readed = 0;
        }
        head += answer_read;
    }
    
    
    return header_readed;
}