/*************************************************************************
	> File Name: tcp_client.h
	> Author: 
	> Mail: 
	> Created Time: 2020年03月29日 星期日 09时44分34秒
 ************************************************************************/

#ifndef _TCP_CLIENT_H
#define _TCP_CLIENT_H
int socket_connect(char *host, int port);
int socket_connect_timeout(char *host, int port, long timeout);

#endif
