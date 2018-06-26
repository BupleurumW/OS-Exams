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
    //������ԭ���ӡ����
        perror("open");
        exit(1);
    }
    if((tofd = open(to,O_WRONLY | O_CREAT | O_TRUNC,S_IRUSR | S_IWUSR )) == -1){
        perror("open to");//������ԭ���ӡ����
        exit(1);
    }
    nread = read(fromfd,buf,sizeof(buf));
     
    while(nread > 0){//��nread>0��Ҳ���ǻ�������û�����ʱ�򣬼���ѭ��
    //��buf�������е�nread���ֽڵ�����д���ļ�������Ϊtofd���ļ���
    //write():ִ�гɹ�ʱ�����ص���ʵ��д�����ݵ��ֽ�������ʧ��ʱ������-1������errnoΪ������
    //��д������ݺ�Ҫд������ݲ�����ʱ��
        if(write(tofd,buf,nread)!=nread){
            printf("write %s error\n",to);
        }
    
        nread = read(fromfd,buf,sizeof(buf));//������ȡ���� 
	}
    if(nread == -1){
        printf("write %s error\n",to);
    }
    close(fromfd);
    close(tofd);
    return;
}

int main(int argc,char *argv[ ]){
    if(argc!=3){//�������Ĳ���������3ʱ
        printf("Usage:%s fromfilename tofilename\n",argv[0]);//��������ִ��ʱ����ʾ
        exit(1);
    }
    //����copy()������������ĵ�һ��������Ӧ���ļ����Ƶ��ڶ����������ļ�
    copy(argv[1],argv[2]);
    return 0;
}

