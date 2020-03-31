/*************************************************************************
	> File Name: client.c
	> Author: 
	> Mail: 
	> Created Time: 2020年03月31日 星期二 09时40分15秒
 ************************************************************************/

#include "../common/chatroom.h"
#include "../common/tcp_client.h"
#include "../common/common.h"

char *conf = "./client.conf";

int main() {
    int port, sockfd;
    struct Msg msg;
    char ip[20] = {0};
    port = atoi(get_value(conf, "SERVER_PORT"));
    strcpy(ip, get_value(conf, "SERVER_IP"));

    //printf("ip = %s, port = %d\n", ip, port);
    if ((sockfd = sockfd_connect(ip, port)) < 0) {
        perror("sockfd_connect");
        return 1;
    }
    strcpy(msg.from, get_value(conf, "MY_NAME"));
    msg.flag =2;

    if (chat_send(msg, sockfd) < 0) {
        return 2;
    }

}

