/*************************************************************************
	> File Name: client.c
	> Author: 
	> Mail: 
	> Created Time: 2020年03月31日 星期二 09时40分15秒
 ************************************************************************/
#include "../common/chatroom.h"
#include "../common/tcp_client.h"
#include "../common/common.h"
#include "../common/color.h"

char *conf = "./client.conf";
int sockfd;


char logfile[50] = {0};
void logout(int signalnum) {
    close(sockfd);
    printf("您已退出.\n");
    exit(1);
}



int main() {
    int port;
    struct Msg msg;
    char ip[20] = {0};
    port = atoi(get_value(conf, "SERVER_PORT"));
    strcpy(ip, get_value(conf, "SERVER_IP"));
    strcpy(logfile, get_value(conf, "LOG_FILE"));
    //printf("ip = %s, port = %d\n", ip, port);
    if ((sockfd = socket_connect(ip, port)) < 0) {
        perror("sockfd_connect");
        return 1;
    }
    strcpy(msg.from, get_value(conf, "MY_NAME"));
    msg.flag =2;

    if (chat_send(msg, sockfd) < 0) {
        return 2;
    }

    struct RecvMsg rmsg = chat_recv(sockfd);
    if (rmsg.retval < 0) {
        fprintf(stderr, "Error!\n");
        return 1;
    }

    printf(GREEN"Server "NONE": %s", rmsg.msg.message);
    if (rmsg.msg.flag == 3){ 
        close(sockfd);
        return 1;
    }
    signal(SIGINT, logout);
    pid_t pid;
    if ((pid = fork()) < 0) {
        perror("fork");
    }
    if (pid == 0) {
        sleep(2);
        system("clear");
        char c = 'a';
        while (c != EOF) {
            printf(L_PINK"Please Input Message:"NONE"\n");
            scanf("%[^\n]s", msg.message);
            c = getchar();
            msg.flag = 0; // 默认是公聊
            if (msg.message[0] == '@') {
                msg.flag = 1; // 私聊
            }
            chat_send(msg, sockfd);
            memset(msg.message, 0, sizeof(msg.message));
            system("clear");
            // 实现查看人数
            if (rmsg.msg.message[0] == '#') {
                printf("输出人数\n");
                if (rmsg.msg.message[1] == '1') {
                    printf("当前在线人数：%d\n", num);
                } else if (rmsg.msg.message[1] == '2') {
                    
                }
            }
        }
        close(sockfd);
    } else {
        //父进程接收服务器转发的所有消息
        //FILE *log_fp = fopen(logfile, "w");
        freopen(logfile, "a+", stdout);
        while (1) {
            rmsg = chat_recv(sockfd);
            if (rmsg.msg.flag == 0) {
                printf(L_BLUE"%s"NONE" : %s\n", rmsg.msg.from, rmsg.msg.message);
            } else if (rmsg.msg.flag == 2) {
                printf(YELLOW"通知信息："NONE "%s\n", rmsg.msg.message);
            } else if (rmsg.msg.flag == 1) {
                printf("私聊信息\n");
            } else {
                printf("Error!\n");
            }
            fflush(stdout);
            wait(NULL);
            close(sockfd);
        }
        return 0;
    }
}

