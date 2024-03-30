#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if(argc < 2){
        printf("Usage: %s <seconds>\n",argv[0]);
        exit(1);
    }
    int pid = fork();
    if(pid > 0){
        wait(0);
        exit(0);
    }else if(pid == 0){
        int seconds = atoi(argv[1]);
        sleep(seconds);
        exit(0);
    }else{
        printf("fork error.\n");
        exit(1);
    }
}
