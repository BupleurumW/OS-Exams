/*
pc1.c: 使用条件变量解决生产者、计算者、消费者问题

系统中有3个线程：生产者、计算者、消费者
系统中有2个容量为4的缓冲区：buffer1、buffer2
生产者生产'a'、'b'、'c'、‘d'、'e'、'f'、'g'、'h'八个字符，放入到buffer1
计算者从buffer1取出字符，将小写字符转换为大写字符，放入到buffer2
消费者从buffer2取出字符，将其打印到屏幕上
*/
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define CAPACITY 4
int buffer1[CAPACITY];
int buffer2[CAPACITY];
int in[2];
int out[2];

int buffer_is_empty(int *p){
	if(p == buffer1)	return in[0]==out[0];
	else 	return in[1]==out[1];
}
int buffer_is_full(int *p){
	if(p == buffer1)
		return (in[0]+1)%CAPACITY==out[0];
	else
		return (in[1]+1)%CAPACITY==out[1];
}
int get_item(int *p){
	int item;
	if(p == buffer1){
		item = buffer1[out[0]];
		out[0] = (out[0]+1)%CAPACITY;
	}else{
		item = buffer2[out[1]];
		out[1] = (out[1]+1)%CAPACITY;
	}
	return item;
}
void put_item(int *p,int item){
	if(p == buffer1){
		buffer1[in[0]] = item;
		in[0] = (in[0]+1)%CAPACITY;
	}else{
		buffer2[in[1]] = item;
		in[1] = (in[1]+1)%CAPACITY;
	}
}

pthread_mutex_t mutex1;
pthread_cond_t wait_empty_buffer1;
pthread_cond_t wait_full_buffer1;
pthread_mutex_t mutex2;
pthread_cond_t wait_empty_buffer2;
pthread_cond_t wait_full_buffer2;

#define ITEM_COUNT (CAPACITY * 2)
void *consume(void *arg)
{
    int i;
	int item;
    for (i = 0; i < ITEM_COUNT; i++) {
        pthread_mutex_lock(&mutex2);
        while (buffer_is_empty(buffer2)){
        	pthread_cond_wait(&wait_full_buffer2, &mutex2);
		}
		item = get_item(buffer2);
    	printf("consume:%c\n",item);
        pthread_cond_signal(&wait_empty_buffer2);
        pthread_mutex_unlock(&mutex2);
    }
    return NULL;
}

void *computer(void *arg){
	int i;
	int item;
	for(i=0;i<ITEM_COUNT;i++){
		pthread_mutex_lock(&mutex1);
		while(buffer_is_empty(buffer1)){
			pthread_cond_wait(&wait_full_buffer1,&mutex1);
		}
		item = get_item(buffer1);
		pthread_mutex_lock(&mutex2);
		while(buffer_is_full(buffer2)){
			pthread_cond_wait(&wait_empty_buffer2,&mutex2);
		}
		item = item - 32;
		printf("computer item: %c to %c\n", item+32,item);
		put_item(buffer2,item);
		pthread_cond_signal(&wait_empty_buffer1);
		pthread_mutex_unlock(&mutex1);
		pthread_cond_signal(&wait_full_buffer2);
		pthread_mutex_unlock(&mutex2);
		
	}
}

void produce()
{
    int i;
	int item;
    for (i = 0; i < ITEM_COUNT; i++) {
        pthread_mutex_lock(&mutex1);
        while (buffer_is_full(buffer1)) 
            pthread_cond_wait(&wait_empty_buffer1, &mutex1);

        item = i + 'a';
        printf("produce item: %c\n", item);
        put_item(buffer1,item);

        pthread_cond_signal(&wait_full_buffer1);
        pthread_mutex_unlock(&mutex1);
    }
}

int main()
{
    pthread_t consumer_tid;
    pthread_t computer_tid;

    pthread_mutex_init(&mutex1, NULL);
    pthread_cond_init(&wait_empty_buffer1, NULL);
    pthread_cond_init(&wait_full_buffer1, NULL);
    pthread_mutex_init(&mutex2, NULL);
    pthread_cond_init(&wait_empty_buffer2, NULL);
    pthread_cond_init(&wait_full_buffer2, NULL);
	pthread_create(&computer_tid, NULL, computer, NULL);
    pthread_create(&consumer_tid, NULL, consume, NULL);
    
	produce();
    pthread_join(computer_tid, NULL);
    pthread_join(consumer_tid, NULL);
   
    return 0;
}
