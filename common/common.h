/*************************************************************************
	> File Name: common.h
	> Author: 
	> Mail: 
	> Created Time: 2020年03月30日 星期一 22时25分44秒
 ************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H
void make_nonblock_ioctl(int fd);
void make_block_ioctl(int fd);
void make_nonblock(int fd);
void make_block(int fd);
#endif
