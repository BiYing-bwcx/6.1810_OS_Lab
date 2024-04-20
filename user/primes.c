#include "kernel/types.h"
#include "user/user.h"

void prime(int x) {
    int num = 0;
    int len = read(x, &num, sizeof(num));
    //printf("len %d\n", len);
    if (len <= 0) {
        close(x);
        return;
    }
    int p[2];
    pipe(p);
    int pid = fork();

    if (pid > 0) { // parent process
        printf("prime %d\n", num);
        int newNum = 0;
        close(p[0]);
        while (read(x, &newNum, sizeof(newNum)) > 0) {
            if (newNum % num != 0)
                write(p[1], &newNum, sizeof(newNum));
        }
        close(p[1]);
        wait(0);
        exit(0);
    } else if (pid == 0) { // child process
        close(p[1]);
        prime(p[0]);
        close(p[0]);
        exit(0);
    } else {
        printf("fork error.\n");
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    int p[2];
    pipe(p);
    int pid = fork();

    if (pid == 0) { // child process
        close(p[1]);
        prime(p[0]);
        close(p[0]);
        exit(0);
    } else if (pid > 0) { // parent process
        close(p[0]);
        for (int i = 2; i <= 35; i++)
            write(p[1], &i, sizeof(i));
        close(p[1]);
        wait(0);
        exit(0);
    } else {
        printf("fork error.\n");
        exit(1);
    }
}