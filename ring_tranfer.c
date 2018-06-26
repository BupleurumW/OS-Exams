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
