#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#define BUFSIZE 1024

void copy(char *from,char *to){
    int fromfd = -1,tofd = -1;
    ssize_t nread;
    char buf[BUFSIZE];
    if((fromfd = open(from,O_RDONLY)) == -1){
    //将出错原因打印出来
        perror("open");
        exit(1);
    }
    if((tofd = open(to,O_WRONLY | O_CREAT | O_TRUNC,S_IRUSR | S_IWUSR )) == -1){
        perror("open to");//将出错原因打印出来
        exit(1);
    }
    nread = read(fromfd,buf,sizeof(buf));
     
    while(nread > 0){//当nread>0，也就是还有数据没读完的时候，继续循环
    //将buf缓冲区中的nread个字节的数据写入文件描述符为tofd的文件中
    //write():执行成功时，返回的是实际写入数据的字节数；当失败时，返回-1，并且errno为错误码
    //当写入的数据和要写入的数据不符的时候
        if(write(tofd,buf,nread)!=nread){
            printf("write %s error\n",to);
        }
    
        nread = read(fromfd,buf,sizeof(buf));//继续读取内容 
	}
    if(nread == -1){
        printf("write %s error\n",to);
    }
    close(fromfd);
    close(tofd);
    return;
}

int main(int argc,char *argv[ ]){
    if(argc!=3){//如果传入的参数不等于3时
        printf("Usage:%s fromfilename tofilename\n",argv[0]);//给出程序执行时的提示
        exit(1);
    }
    //调用copy()函数，将传入的第一个参数对应的文件复制到第二个参数的文件
    copy(argv[1],argv[2]);
    return 0;
}

