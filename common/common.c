/*************************************************************************
	> File Name: common.c
	> Author: 
	> Mail: 
	> Created Time: 2020年03月30日 星期一 22时23分00秒
 ************************************************************************/

#include "head.h"


void make_nonblock_ioctl(int fd) {
    unsigned long ul = 1; // 非阻塞
    ioctl(fd, FIONBIO, &ul);
}

void make_block_ioctl(int fd) {
    unsigned long ul = 0; // 塞
    ioctl(fd, FIONBIO, &ul);
}

void make_nonblock(int fd) {
    int flag;
    if ((flag = fcntl(fd, F_GETFL, 0)) < 0) {
        return;
    }
    flag |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flag);

}

void make_block(int fd) {
    int flag;
    if ((flag = fcntl(fd, F_GETFL, 0)) < 0) {
        return ;
    }
    flag &= ~O_NONBLOCK;
    fcntl(fd, F_SETFL, flag);
}
