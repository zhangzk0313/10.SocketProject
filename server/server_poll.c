/*************************************************************************
	> File Name: server_poll.c
	> Author: zzk
	> Mail: 815028387@qq.com
	> Created Time: 2020年04月11日 星期六 18时13分56秒
 ************************************************************************/

#include<stdio.h>
#include "../common/head.h"
#include "../common/tcp_server.c"

#define POLLSIZE 100
#define BUFSIZE 512

char ch_char (char c) {
    if (c >= 'a' && c <= 'z')
        return c - 32;
    return c;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s port!\n", argv[0]);
        exit(1);
    }
    int server_listen, fd;
    if ((server_listen = socket_create(atoi(argv[1]))) < 0) {
        perror("socket_create");
        exit(1);
    }

    struct pollfd event_set[POLLSIZE];

    for (int i = 0; i < POLLSIZE; i++) {
        event_set[i].fd = -1;
    }
    event_set[0].fd = server_listen;
    event_set[0].events = POLLIN;

    while (1) {
        int retval;
        if (retval = poll(event_set, POLLSIZE, -1) < 0) {
            perror("poll");
            return 1;
        }
        if (event_set[0].revents & POLLIN) {
            if ((fd = accept(server_listen, NULL, NULL)) < 0) {
                perror("accept");
                continue;
            }
            retval--;
            int i;
            for (int i = 1; i < POLLSIZE; i++) {
                if (event_set[i].fd < 0) {
                    event_set[i].fd = fd;
                    event_set[i].events = POLLIN;
                    break;
                }
            }
            if (i == POLLSIZE) {
                printf("Too many clients\n");
            }
        }
        for (int i = 1; i < POLLSIZE; i++) {
            if (event_set[i].fd < 0) continue;
            if (event_set[i].revents & (POLLIN | POLLHUP | POLLERR)) {
                retval--;
                char buff[BUFSIZE] = {0};
                if (recv(event_set[i].fd, buff, BUFSIZE, 0) > 0) {
                    printf("Recv: %s\n", buff);
                    for (int i = 0; i < strlen(buff); i++) {
                        buff[i] = ch_char(buff[i]);
                    }
                    send(event_set[i].fd, buff, strlen(buff), 0);
                } else {
                    close(event_set[i].fd);
                    event_set[i].fd = -1;
                }
            }
            if (retval <= 0) break;
        }
    }
    return 0;
}
