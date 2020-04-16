/*************************************************************************
	> File Name: tcp_client.h
	> Author: zhangzk
	> Mail: 815028387@qq.com
	> Created Time: 六  3/28 14:34:41 2020
 ************************************************************************/

#ifndef _TCP_CLIENT_H
#define _TCP_CLIENT_H
int socket_connect(char *host, int port);
int socket_connect_timeout(char *host, int port, long timeout);
#endif

