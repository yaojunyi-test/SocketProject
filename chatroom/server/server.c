/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: 2020年03月30日 星期一 22时04分48秒
 ************************************************************************/

#include "../common/common.h"
#include "../common/tcp_server.h"
#include "../common/chatroom.h"

struct User {
    char name[20];
    int online;
    pthread_t tid;
    int fd;
};

char *conf = "./server.conf";
struct User *client;

void *work(void *arg) {
    printf("client login!\n");

}

int find_sub() {
    for (int i = 0; i < MAX_CLIENT; i++) {
        if (!client[i].online) return i;
    }
    return -1;
}

bool check_online(char *name) {
    for (int i = 0; i < MAX_CLIENT; i++) {
        if (client[i].online && !strcmp(name, client[i].name)) {
            printf("D: %s is online\n", name);
            return true;
        }
    }
    return false;
}


int main() {
    int port, server_listen, fd;
    struct RecvMsg recvmsg; //用来收信息
    struct Msg msg;
    port = atoi(get_value(conf, "SERVER_PORT"));
    client = (struct User *)calloc(MAX_CLIENT, sizeof(struct User));
    
    if ((server_listen = socket_create(port)) < 0) {
        perror("socket_create");
        return 1;
    }
    while (1) {
        if ((fd = accept(server_listen, NULL, NULL)) < 0) {
            perror("accept");
            continue;
        }
        recvmsg = chat_recv(fd);
        if (recvmsg.retval < 0) {
            close(fd);
            continue;
        }
        
        if (check_online(recvmsg.msg.from)) {
            //拒绝连接
        } else {
            int sub;
            sub = find_sub();
            client[sub].online = 1; // 1是在线
            client[sub].fd = fd;
            strcpy(client[sub].name, recvmsg.msg.from);
            pthread_create(&client[sub].tid, NULL, work, NULL);
        }
    //printf("%d\n", port);
    }
    

    return 0;
}

