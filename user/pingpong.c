#include"kernel/types.h"
#include"user/user.h"

int
main(int argc, char *argv[]){
    int p1[2];
    int p2[2];
    pipe(p1);
    pipe(p2);
    char buf[1];
    buf[0] = 'A';
    int pid = fork();
    if(pid > 0){
        close(p1[0]);
        close(p2[1]);
        write(p1[1],buf,1);
        close(p1[1]);
        wait(0);
        read(p2[0],buf,1);
        close(p2[0]);
        printf("%d: received pong\n",getpid());
        exit(0);
    }else if(pid == 0){
        close(p1[1]);
        close(p2[0]);
        read(p1[0],buf,1);
        close(p1[0]);
        write(p2[1],buf,1);
        close(p2[1]);
        printf("%d: received ping\n",getpid());
        exit(0);
    }else {
        printf("fork error.\n");
        exit(1);
    }
}