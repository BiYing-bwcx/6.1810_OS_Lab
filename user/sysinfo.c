#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/sysinfo.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    // param error
    if (argc != 1)
    {
        fprintf(2, "Usage: %s need not param\n", argv[0]);
        exit(1);
    }

    int pid = fork();
    struct sysinfo info;
    if(pid == 0){
        sysinfo(&info);
        // print the sysinfo
        printf("free space: %d\nused process: %d\n", info.freemem, info.nproc);
        exit(0);
    }else if(pid > 0){
        wait(0);
        exit(0);
    }else{
        fprintf(2, "Error: fork() failed.\n");
        exit(1);
    }
}
