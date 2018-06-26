/*
pc2.c: 使用信号量解决生产者、计算者、消费者问题
系统中有3个线程：生产者、计算者、消费者
系统中有2个容量为4的缓冲区：buffer1、buffer2
生产者生产'a'、'b'、'c'、‘d'、'e'、'f'、'g'、'h'八个字符，放入到buffer1
计算者从buffer1取出字符，将小写字符转换为大写字符，放入到buffer2
消费者从buffer2取出字符，将其打印到屏幕上
*/
//信号量和互斥锁(mutex)的区别：互斥锁只允许一个线程进入临界区，而信号量允许多个线程同时进入临界区。
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

//储存变量
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

//定义两个buff的互斥信号量
sem_t buff1_mutex;
sem_t buff2_mutex;
//定义进程内的互斥信号量
sem_t buff1_full;
sem_t buff1_empty;
sem_t buff2_full;
sem_t buff2_empty;

void init_mutex(){
	/*int sem_init(sem_t *sem,int pshared,unsigned int value); 
	sem_init函数是Posix信号量操作中的函数。sem_init() 初始化一个定位在 sem 的匿名信号量。value 参数指定信号量的初始值。 
	pshared 参数指明信号量是由进程内线程共享，还是由进程之间共享。
	如果 pshared 的值为 0，那么信号量将被进程内的线程共享，并且应该放置在这个进程的所有线程都可见的地址上 */
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
	//int sem_wait(sem_t *sem);等待信号量，如果信号量的值大于0,将信号量的值减1,立即返回。
	//如果信号量的值为0,则线程阻塞。相当于P操作。成功返回0,失败返回-1。

	//int sem_post(sem_t *sem); 释放信号量，让信号量的值加1。相当于V操作。
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
