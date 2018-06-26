#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

int  filecp(int src,int dest)   {  //读取文件中的内容，并输出到屏幕上
    int c; 
    char buf[1024];
    while(c>0){
    	if((write,dest,buf,c) != c){
    		printf("write error!\n");
		}
		c = read(src,buf,sizeof(buf));
	}             
    return c;  
}  
int main(int argc,char *argv[])  {  
    if(argc==1){ //如果没有输入参数
         filecp(stdin,stdout);        
    }
	else{  
        char *in;    
        int inf;  
        while(--argc>0){  //对于输入的参数，读取内容，并输出到屏幕上
            in = argv[argc];         
            if((inf=open(in,)_RDONLY))==-1){//如果某个文件无法打开则跳过   
                printf("\ncan't open %s\n",in);  
                continue;                              
            }   
            filecp(inf,stdout);  
            fclose(inf);  
        }
       return 0;    
    }  
}  
