#include <stdio.h>
#include <stdlib.h> 
#include <errno.h>
#include <stdlib.h>

int my_system(const char *cmd){
	int stat;
	pid_t pid;
	if(command == NULL)	return (1);
	
	if((pid = fork()) == 0){//子进程 
		execl("/bin/sh","sh","-c",command,(char *)0);
		_exit(127); 
	}
	
	if(pid == -1){
		//创建失败 
		stat = -1;
	}else{
		while(waitpid(pid,&status,0) == -1){
			if(errno != EINTER){
				stat = -1;
				break;
			}
		}
	} 
	return status;
}

void execute(char *command)
{
    my_system(command);
    printf(">> Command '%s' finished!\n\n", command);
}

int main()
{
    execute("ls /");
    execute("echo HelloWorld");
    return 0;
}
