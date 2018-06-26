/*
pc2.c: ʹ���ź�����������ߡ������ߡ�����������
ϵͳ����3���̣߳������ߡ������ߡ�������
ϵͳ����2������Ϊ4�Ļ�������buffer1��buffer2
����������'a'��'b'��'c'����d'��'e'��'f'��'g'��'h'�˸��ַ������뵽buffer1
�����ߴ�buffer1ȡ���ַ�����Сд�ַ�ת��Ϊ��д�ַ������뵽buffer2
�����ߴ�buffer2ȡ���ַ��������ӡ����Ļ��
*/
//�ź����ͻ�����(mutex)�����𣺻�����ֻ����һ���߳̽����ٽ��������ź����������߳�ͬʱ�����ٽ�����
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

//�������
int buffer1[5];
int buffer2[5];
int in[2];
int out[2];
int get_item(int *p){
	int item;
	if(p == buffer1){
		item = buffer1[out[0]];
		out[0] = (out[0]+1)%4;
	}else{
		item = buffer2[out[1]];
		out[1] = (out[1]+1)%4;
	}
	return item;
}
void put_item(int *p,int item){
	if(p == buffer1){
		buffer1[in[0]] = item;
		in[0] = (in[0]+1)%4;
	}else{
		buffer2[in[1]] = item;
		in[1] = (in[1]+1)%4;
	}
}

//��������buff�Ļ����ź���
sem_t buff1_mutex;
sem_t buff2_mutex;
//��������ڵĻ����ź���
sem_t buff1_full;
sem_t buff1_empty;
sem_t buff2_full;
sem_t buff2_empty;

void init_mutex(){
	/*int sem_init(sem_t *sem,int pshared,unsigned int value); 
	sem_init������Posix�ź��������еĺ�����sem_init() ��ʼ��һ����λ�� sem �������ź�����value ����ָ���ź����ĳ�ʼֵ�� 
	pshared ����ָ���ź������ɽ������̹߳��������ɽ���֮�乲��
	��� pshared ��ֵΪ 0����ô�ź������������ڵ��̹߳�������Ӧ�÷�����������̵������̶߳��ɼ��ĵ�ַ�� */
    sem_init(&buff1_mutex,0,1);
    sem_init(&buff2_mutex,0,1);
    sem_init(&buff1_full,0,0);
    sem_init(&buff1_empty,0,4);
    sem_init(&buff2_full,0,0);
    sem_init(&buff2_empty,0,4);
}
void destroy_mutex(){
    sem_destroy(&buff1_mutex);
    sem_destroy(&buff2_mutex);
    sem_destroy(&buff1_full);
    sem_destroy(&buff1_empty);
    sem_destroy(&buff2_full);
    sem_destroy(&buff2_empty);
}


void *producer(void *arg){
	//int sem_wait(sem_t *sem);�ȴ��ź���������ź�����ֵ����0,���ź�����ֵ��1,�������ء�
	//����ź�����ֵΪ0,���߳��������൱��P�������ɹ�����0,ʧ�ܷ���-1��

	//int sem_post(sem_t *sem); �ͷ��ź��������ź�����ֵ��1���൱��V������
    int i;
    int item;
    for(i=0;i<8;i++){
        sem_wait(&buff1_empty);//P -1
        sem_wait(&buff1_mutex);
        item = i+'a';
        printf("producer:%c \n",item);
        put_item(buffer1,item); 
        sem_post(&buff1_mutex);//V +1
        sem_post(&buff1_full);
    }
}

void *computer(void *arg){
    int i;
    int item;
    for(i=0;i<8;i++){
        sem_wait(&buff1_full);
        sem_wait(&buff2_empty);
        sem_wait(&buff1_mutex);
        sem_wait(&buff2_mutex);
        item = get_item(buffer1);
        item = item - 32;
        printf("computer:%c->%c \n",item+32,item);
        put_item(buffer2,item);
        sem_post(&buff2_mutex);
        sem_post(&buff1_mutex);
        sem_post(&buff2_full);
        sem_post(&buff1_empty);
    }
}
void *consumer(void *arg){
    int i;
    int item;
    for(i=0;i<8;i++){
        sem_wait(&buff2_full);
        sem_wait(&buff2_mutex);
        item = get_item(buffer2);
        printf("consumer:%c \n",item);
        sem_post(&buff2_mutex);
        sem_post(&buff2_empty);
    }
}

int main(){
    init_mutex();
	pthread_t tid[3];
	pthread_create(&tid[0],NULL,producer,NULL);
	pthread_create(&tid[1],NULL,computer,NULL);
	pthread_create(&tid[2],NULL,consumer,NULL);
	int i;
	for(i=0;i<3;i++){
		pthread_join(tid[i],NULL);
	}
	destroy_mutex();
    return 0;
}
