/*************************************************************************
	> File Name: out.c
	> Author: 
	> Mail: 
	> Created Time: 2020年04月04日 星期六 15时14分16秒
 ************************************************************************/

#include <stdio.h> 
#include <string.h>
#include <sys/file.h>

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s filename", argv[0]);
        return 0;
    }
    FILE *fp;
    pid_t pid;
    int cnt = argv[1];
    int pid_num = argv[2];
    

    for (int i = 0; i <= cnt; i++) {
        fp = fopen("now.c", O_RDWR|O_CREAT, 0777);
        fp = fopen("sum.c", O_RDWR|O_CREAT, 0777);
        flock(fp->_file, LOCK_EX);
        if ((pid = fork()) < 0) {
            perror("fork");
            exit(1);
        }
        char buffer[512] = {0};
        fread(buffer, 512, 1, fp);
        int *sub;
        int num[10];
        strcpy(num, sub + 4);
        fwrite();

        sub = strstr(buffer, "num");

        fwrite(buffer, , 1, fp);


        flock(fp->_fileno, LOCK_UN);

        flock(fp->_fileno, LOCK_EX);
        if ((pid = fork()) < 0) {
            perror("fork");
            exit(1);
        }


        flock(fp->_fileno, LOCK_UN);

        flock(fp->_fileno, LOCK_EX);
        if ((pid = fork()) < 0) {
            perror("fork");
            exit(1);
        }


        flock(fp->_fileno, LOCK_UN);




        
    }

    return 0;
}

