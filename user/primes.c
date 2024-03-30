#include"kernel/types.h"
#include"user/user.h"

void
prime(int num, int x){
    int p[2];
    pipe(p);
    int pid = fork();
    if(pid > 0){
        int newNum = 0;
        int len = read(x,&newNum,sizeof(newNum));
        //printf("prime %d\n",newNum);
        while(len > 0){
            if(newNum % num != 0){
                //printf("newNum : %d\n",newNum);
                write(p[1],&newNum,sizeof(newNum));
            }
            newNum = 0;
            len = read(x,&newNum,sizeof(newNum));
        }
        close(p[0]);
        close(p[1]);
        wait(0);
        exit(0);
    }else if(pid == 0){
        int num = 0;
        read(p[0],&num,sizeof(num));
        printf("prime %d\n",num);
        prime(num,p[0]);
        close(p[1]);
        close(p[0]);
        exit(0);
    }else{
        printf("fork error.\n");
        exit(1);
    }
}
int 
main(int argc, char *argv[]){
    int p[2];
    pipe(p);
    int pid = fork();
    if(pid == 0){
        int num = 0;
        close(p[1]);
        read(p[0],&num,sizeof(num));
        printf("prime %d\n",num);
        prime(num,p[0]);
        close(p[0]);
        exit(0);
    }else if(pid > 0){
        close(p[0]);
        for(int i = 2; i <= 35; i++){
            write(p[1],&i,sizeof(i));
        }
        close(p[1]);
        wait(0);
        exit(0);
    }else{
        printf("fork error.\n");
        exit(1);
    }
}