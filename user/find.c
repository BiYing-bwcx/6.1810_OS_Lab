#include"kernel/types.h"
#include"kernel/stat.h"
#include"kernel/fs.h"
#include"user/user.h"

void
find_file(char *path, char *c){
    char buf[512],*p;
    int fd;
    struct dirent de;//dirent
    struct stat st;
    if((fd = open(path,0)) < 0){//打开路径
        fprintf(2,"find: cannot open %s\n",path);
        return;
    }
    if(fstat(fd,&st) < 0){//返回文件夹信息
        fprintf(2,"find: cannot stat %s\n",path);
        close(fd);
        return;
    }
    if(st.type != T_DIR){//如果不是文件夹
        fprintf(2,"find: %s not is a dir.\n",path);
        close(fd);
        return;
    }
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){//如果路径过长
        fprintf(2,"find: dir too long\n");
        close(fd);
        return;
    }

    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';//更新路径到下一级
    //printf("buf: %s\n",buf);
    while(read(fd,&de,sizeof(de)) == sizeof(de)){//de是文件名和索引点inum
        if(de.inum == 0) continue;
        if(!strcmp(de.name,".") || !strcmp(de.name,"..")) continue;
        //printf("de.name: %s\n",de.name);
        memmove(p,de.name,sizeof(de.name));
        //printf("buf2: %s\n",buf);
        if(stat(buf,&st) < 0){
            fprintf(2,"find: connot stat %s.\n",buf);
            continue;
        }
        if(st.type == T_DIR) {
            //if(!strcmp(de.name,c)) printf("%s\n",buf);
            find_file(buf,c);//st是文件信息，大小类型那个等等
        }
        else if(st.type == T_FILE){
            if(!strcmp(de.name,c))
                printf("%s\n",buf);
        }
    }
}

int
main(int argc, char *argv[]){
    if(argc < 3){
        fprintf(2,"Using error.\n");
        exit(1);
    }
    int pid = fork();
    if(pid > 0){
        wait(0);
        exit(0);
    }else if(pid == 0){
        find_file(argv[1],argv[2]);
        exit(0);
    }else{
        printf("fork error.\n");
        exit(1);
    }
}