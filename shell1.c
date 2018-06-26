#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CMD 100
#define MAX_DIR_NAME 100

int buildin_command(char **argv){  //执行内置指令 
	if(strcmp(argv[0],"exit") == 0){
		exit(0);
	}else if(strcmp(argv[0],"cd") == 0){
		if(chdir(argv[1])){
			//切换目录失败
			printf("cd： %s no such directory\n",argv[1]); 
		}
		return 1;
		
	}else if(strcmp(argv[0],"pwd") == 0){
		char buf[MAX_DIR_NAME];
		printf("%s\n",getcwd(buf,sizeof(buf))); 
		return 1;
		
	}
	return 0;
}

int parseline(char *buf,char **argv){ 
    while(*buf==' '){
        buf++;
    }
    int delim = 0;
    int argc = 0;
    while(*buf!='\n'){
        while(buf[delim]!='\n'&&buf[delim]!=' '){
            delim++;
        }
        if(buf[delim] == '\n'){
            buf[delim] = '\0';
            argv[argc++] = buf;
            break;
        }
        buf[delim] = '\0';
        argv[argc++] = buf; 
        buf+=delim+1;
        delim = 0;
        while(*buf==' '){
            buf++;
        }
    }
    argv[argc] = NULL;
    return 0;
}

void eval(char *cmdstring){
	char buf[MAX_CMD];
	char *argv[MAX_CMD];
	strcpy(buf,cmdstring);
	parseline(cmdstring,argv);//将用户输入拆分成字符串数组 
	if(buildin_command(argv) == 1)	return ;//如果是内置指令则直接返回 
	int pid = fork();//外置指令则开进程 
	if(pid == 0){//子进程
		if(execvp(argv[0],argv) < 0){
			printf("%s:command not found\n",argv[0]);
			exit(0);
		} 
	}
	wait(pid);
}

int main(){
	char cmdstring[MAX_CMD];
	while(1){
		printf("* Calabsh * >> ");
		fflush(stdout);
		if(read(0,cmdstring,MAX_CMD) < 0){
			printf("read error!");
		}else{
			eval(cmdstring);
		}
	}
	return 0;
}
