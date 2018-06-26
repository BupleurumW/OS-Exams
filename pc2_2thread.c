/*
pc2.c: 使用信号量解决生产者、计算者、消费者问题

系统中有3个线程：生产者、计算者、消费者
系统中有2个容量为4的缓冲区：buffer1、buffer2
生产者生产'a'、'b'、'c'、‘d'、'e'、'f'、'g'、'h'八个字符，放入到buffer1
计算者从buffer1取出字符，将小写字符转换为大写字符，放入到buffer2
消费者从buffer2取出字符，将其打印到屏幕上
*/
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
typedef struct{
	int value;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}sema_t;
void sema_init(sema_t *sema,int value){
	sema->value = value;
	pthread_mutex_init(&sema->mutex,NULL);
	pthread_cond_init(&sema->cond,NULL);
}
void sema_wait(sema_t *sema)
{
    pthread_mutex_lock(&sema->mutex);
    while (sema->value <= 0)
        pthread_cond_wait(&sema->cond, &sema->mutex);
    sema->value--;
    pthread_mutex_unlock(&sema->mutex);
}
void sema_signal(sema_t *sema){
	pthread_mutex_lock(&sema->mutex);
	++sema->value;
	pthread_cond_signal(&sema->cond);
	pthread_mutex_unlock(&sema->mutex);
}

#define CAPACITY 4
int buffer1[CAPACITY];
int buffer2[CAPACITY];
int in[2];
int out[2];
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
sema_t mutex_sema[2];
sema_t empty_buffer_sema[2];
sema_t full_buffer_sema[2];
#define ITEM_COUNT (CAPACITY * 2)
void *consume(void *arg)
{
    int i;
	int item;
    for (i = 0; i < ITEM_COUNT; i++) {
    	sema_wait(&full_buffer_sema[1]);
        sema_wait(&mutex_sema[1]);
        item = get_item(buffer2);
		printf("consume:%c\n",item);
        sema_signal(&empty_buffer_sema[1]);
        sema_signal(&mutex_sema[1]);
		
    }
    return NULL;
}

void *computer(void *arg){
	int i;
	int item;
	for(i=0;i<ITEM_COUNT;i++){
		sema_wait(&full_buffer_sema[0]);
		sema_wait(&mutex_sema[0]);
		item = get_item(buffer1);
		sema_signal(&empty_buffer_sema[0]);
		sema_signal(&mutex_sema[0]);
		sema_wait(&empty_buffer_sema[1]);
		sema_wait(&mutex_sema[1]);
		item = item - 32;
		printf("computer item: %c to %c\n", item+32,item);
		put_item(buffer2,item);
		sema_signal(&full_buffer_sema[1]);
		sema_signal(&mutex_sema[1]);
		
		
	}
}

void produce()
{
    int i;
	int item;
    for (i = 0; i < ITEM_COUNT; i++) {
    	sema_wait(&empty_buffer_sema[0]);
        sema_wait(&mutex_sema[0]);
        item = i + 'a';
        put_item(buffer1,item);
		printf("produce item: %c\n", item);
        sema_signal(&full_buffer_sema[0]);
        sema_signal(&mutex_sema[0]);
		
    }
}

int main()
{
    pthread_t consumer_tid;
    pthread_t computer_tid;

    sema_init(&mutex_sema[0],1);
    sema_init(&empty_buffer_sema[0],4);
    sema_init(&full_buffer_sema[0],0);
    sema_init(&mutex_sema[1],1);
    sema_init(&empty_buffer_sema[1],4);
    sema_init(&full_buffer_sema[1],0);
    
	pthread_create(&computer_tid, NULL, computer, NULL);
    pthread_create(&consumer_tid, NULL, consume, NULL);
	produce();
    pthread_join(computer_tid, NULL);
    pthread_join(consumer_tid, NULL);
   
    return 0;
}
