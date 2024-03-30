#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "Usage error.\n");
        exit(1);
    }

    char *argvs[MAXARG]; // 支持最多参数
    int index = 0;

    for (int i = 1; i < argc; i++) {
        argvs[index++] = argv[i];
    }

    char buf;
    char new_argvs[512];
    int buf_index = 0;
    int n;

    while ((n = read(0, &buf, 1)) > 0) {
        if (buf == '\n') {
            new_argvs[buf_index] = 0;
            int pid = fork();
            if (pid < 0) {
                fprintf(2, "fork failed\n");
                exit(1);
            } else if (pid == 0) {
                argvs[argc - 1] = new_argvs;
                printf("argv:%s\n",argvs[argc-1]);
                argvs[argc] = 0;
                exec(argvs[0], argvs);
                fprintf(2, "exec failed\n");
                exit(1);
            } else {
                wait(0); // 等待子进程结束
                buf_index = 0;
            }
        } else {
            new_argvs[buf_index++] = buf;
        }
    }

    exit(0);
}
