/*
ring.c: 创建N个线程，它们构成一个环

创建N个线程：T1、T2、T3、… TN
T1向T2发送整数1
T2收到后将整数加1
T2向T3发送整数2
T3收到后将整数加1
T3向T4发送整数3
…
TN收到后将整数加1
TN向T1发送整数N
*/

#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<malloc.h>

#define N 5

int count = 0;

struct param{
	int sum;
}pa;

void *add(void *arg){
	count++;
	if(count > N){
		return arg;
	}else{
		printf("T%d \n",count);
		struct param *p = (struct param *)arg;
		p->sum++;
		pthread_t id;
		pthread_create(&id,NULL,add,p);
		pthread_join(id,(void **)&p);
		return p;
	}
}

int main(){
	struct param *p = (struct param *)malloc(sizeof(pa));
	p->sum = 0;
	pthread_t worker_tid;
	pthread_create(&worker_tid,NULL,add,p);
	pthread_join(worker_tid,(void **)&p);
	printf("result:%d\n",p->sum);
	return 0;
}
