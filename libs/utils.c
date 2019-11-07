#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

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
        exit_if(answer_read == -1, "read read_client");
        head += answer_read;
        /*
        if (answer_read == 0)
        {
            sleep(1);
        }
        */
    }
    head = 0;
    struct message answer;
    answer.user_id = four_char_to_int(&buffer[0]);
    answer.data_len = four_char_to_int(&buffer[4]);
    return answer;
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


