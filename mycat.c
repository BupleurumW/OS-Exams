#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

int  filecp(int src,int dest)   {  //��ȡ�ļ��е����ݣ����������Ļ��
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
    if(argc==1){ //���û���������
         filecp(stdin,stdout);        
    }
	else{  
        char *in;    
        int inf;  
        while(--argc>0){  //��������Ĳ�������ȡ���ݣ����������Ļ��
            in = argv[argc];         
            if((inf=open(in,)_RDONLY))==-1){//���ĳ���ļ��޷���������   
                printf("\ncan't open %s\n",in);  
                continue;                              
            }   
            filecp(inf,stdout);  
            fclose(inf);  
        }
       return 0;    
    }  
}  
