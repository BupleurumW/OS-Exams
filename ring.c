/*
ring.c: ����N���̣߳����ǹ���һ����

����N���̣߳�T1��T2��T3���� TN
T1��T2��������1
T2�յ���������1
T2��T3��������2
T3�յ���������1
T3��T4��������3
��
TN�յ���������1
TN��T1��������N
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
