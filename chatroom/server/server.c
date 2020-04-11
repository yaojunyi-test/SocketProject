/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: 2020年03月30日 星期一 22时04分48秒
 ************************************************************************/

#include "../common/common.h"
#include "../common/tcp_server.h"
#include "../common/chatroom.h"
#include "../common/color.h"
struct User {
    char name[20];
    int online;
    pthread_t tid;
    int fd;
};

char *conf = "./server.conf";
struct User *client;
struct Msg msg;

int sum = 0; //记录在线人数

void send_all(struct Msg msg) {
    for (int i = 0; i < MAX_CLIENT; i++) {
        if (client[i].online) // 0为不在线
            chat_send(msg, client[i].fd);
    }
}

void send_list(struct Msg msg) {
    char list[512] = {0};
    for (int i = 0; i < MAX_CLIENT; i++) {
        strcpy(list, client[i].name);
        chat_send(msg, client->fd);
    }
}

int check_name(char *name) {
    for (int i = 0; i < MAX_CLIENT; i++) {
        if (client[i].online && !strcmp(client[i].name, name)) 
            return i;
    }
    return -1;
}

void *work(void *arg) {
    int *sub = (int *)arg;
    int client_fd = client[*sub].fd;
    struct RecvMsg rmsg;
    while (1) {
        rmsg = chat_recv(client_fd);
        if (rmsg.retval < 0) {
            printf(PINK"Logout: "NONE" %s \n", client[*sub].name);
            close(client_fd);
            client[*sub].online = 0;
            sum--; //下线
            return NULL;
        }
        printf(BLUE"%s"NONE" : %s\n", rmsg.msg.from, rmsg.msg.message);

        //转发给所有的客户端 0:公聊 
        if (rmsg.msg.flag == 0) {
            send_all(rmsg.msg);
        } else if (rmsg.msg.flag == 1){
            //私聊信息
            if (rmsg.msg.message[0] == '@') {
                char to[20] = {0};
                int i = 1;
                for (; i <= 20; i++) {
                    if (rmsg.msg.message[i] == ' ') 
                        break;
                }
                strncpy(to, rmsg.msg.message + 1, i - 1);
                int ind;
                if ((ind = check_name(to)) < 0) {
                    //告知不在线
                    sprintf(rmsg.msg.message, "%s is not online.", to);
                    rmsg.msg.flag == 2;
                    chat_send(rmsg.msg, client_fd);
                    continue;
                }
                chat_send(rmsg.msg, client[ind].fd);
                // 发送在线人数
                if (rmsg.msg.message[0] == '#') {
                     if (rmsg.msg.message[1] == '1') {
                    sprintf(rmsg.msg.message, "The nums of online is %d.", sum);
                    chat_send(rmsg.msg, client_fd);
                }
                if (rmsg.msg.message[1] == '2') {
                    send_list(rmsg.msg);
                }
                }
            }
        }
    }
    return NULL;
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
            msg.flag = 3;
            strcpy(msg.message, "You Are Already Login System!");
            chat_send(msg, fd);
            close(fd);
            continue;
        }
        msg.flag = 2;
        strcpy(msg.message, "Welcome to this chat room!");
        chat_send(msg, fd);

        int sub, ret;
        sum++;  //上线
        sub = find_sub();
        client[sub].online = 1; // 1是在线
        client[sub].fd = fd;
        strcpy(client[sub].name, recvmsg.msg.from); 
        ret = pthread_create(&client[sub].tid, NULL, work, (void *)&sub);
        if (ret != 0) {
            fprintf(stderr, "pthread_create");
        }
        //printf("%d\n", port);
    }
    return 0;
}
