/*
pc1.c: 使用条件变量解决生产者、计算者、消费者问题

系统中有3个线程：生产者、计算者、消费者
系统中有2个容量为4的缓冲区：buffer1、buffer2
生产者生产'a'、'b'、'c'、‘d'、'e'、'f'、'g'、'h'八个字符，放入到buffer1
计算者从buffer1取出字符，将小写字符转换为大写字符，放入到buffer2
消费者从buffer2取出字符，将其打印到屏幕上  */

#include <stdio.h>
#include <pthread.h>

int buffer1[5];
int buffer2[5];
int index_buff1 = 0;
int index_buff2 = 0;

pthread_mutex_t buff1_mutex;
pthread_mutex_t buff2_mutex;
pthread_cond_t buff1_full_cond;
pthread_cond_t buff1_empty_cond;
pthread_cond_t buff2_full_cond;
pthread_cond_t buff2_empty_cond;

void init_mutex(){
	pthread_mutex_init(&buff1_mutex,NULL);
	pthread_mutex_init(&buff2_mutex,NULL);
	pthread_cond_init(&buff1_empty_cond,NULL);
	pthread_cond_init(&buff1_full_cond,NULL);
	pthread_cond_init(&buff2_empty_cond,NULL);
	pthread_cond_init(&buff2_full_cond,NULL);
}

void destroy_mutex(){
	pthread_mutex_destroy(&buff1_mutex);
    pthread_mutex_destroy(&buff2_mutex);
    pthread_cond_destroy(&buff1_empty_cond);
    pthread_cond_destroy(&buff1_full_cond);
    pthread_cond_destroy(&buff2_empty_cond);
    pthread_cond_destroy(&buff2_full_cond);
}

void *producer(void *arg){
	int i;
	for(i=0;i<8;i++){
		pthread_mutex_lock(&buff1_mutex);
		while(index_buff1 >= 4){
			pthread_cond_wait(&buff1_empty_cond,&buff1_mutex);
		}
		buffer1[index_buff1] = 'a' + i;
		printf("producer: %c\n",buffer1[index_buff1]);
		index_buff1++;
		pthread_cond_signal(&buff1_full_cond);
		pthread_mutex_unlock(&buff1_mutex);
	}
}

void *computer(void *arg){
	int i;
	for(i=0;i<8;i++){
		pthread_mutex_lock(&buff1_mutex);
		pthread_mutex_lock(&buff2_mutex);
		while(index_buff1 < 1){
			pthread_cond_wait(&buff1_full_cond,&buff1_mutex);
		}
		while(index_buff2 >= 4){
			pthread_cond_wait(&buff2_empty_cond,&buff2_mutex);
		}
		index_buff1--;
		buffer2[index_buff2] = buffer1[index_buff1] - 32;
		printf("computer: %c -> %c\n",buffer1[index_buff1],buffer2[index_buff2]);
		index_buff2++;
        pthread_cond_signal(&buff1_empty_cond);
        pthread_cond_signal(&buff2_full_cond);      
        pthread_mutex_unlock(&buff2_mutex);
        pthread_mutex_unlock(&buff1_mutex);
	}
}

void *consumer(void *arg){
	int i;
	for(i=0;i<8;i++){
		pthread_mutex_lock(&buff2_mutex);
		while(index_buff2 < 1){
			pthread_cond_wait(&buff2_full_cond,&buff2_mutex);
		}
		index_buff2--;
		printf("comsumer: %c\n",buffer2[index_buff2]);
		pthread_cond_signal(&buff2_empty_cond);
        pthread_mutex_unlock(&buff2_mutex);
	}
}

int main()
{
	init_mutex();
	pthread_t tid[3];
	pthread_create(&tid[0],NULL,producer,NULL);
	pthread_create(&tid[1],NULL,computer,NULL);
	pthread_create(&tid[2],NULL,consumer,NULL);
	for(int i=0;i<3;i++){
		pthread_join(tid[i],NULL);
	}
	destroy_mutex();
    return 0;
}
