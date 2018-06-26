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
#include<stdlib.h>
#include<pthread.h>

#define N 10

struct param{
	int sum;
};

void *add(void *arg){
	struct param *p = (struct param *)arg;
	printf("T%d\n",pthread_self());
	p->sum++;
	return p;
}

int main(){
	pthread_t add_id[N];
	struct param *p = (struct param *)malloc(sizeof(struct param));
	p->sum = 0;
	int i = 1;
	while(i<=N){
		pthread_create(&add_id[i],NULL,add,p);
		pthread_join(add_id[i],(void **)&p);
		i++;
	}
	
	printf("\nresult:%d",p->sum);
	return 0;
}
