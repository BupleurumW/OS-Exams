/*
pi2.c: ʹ��N���̸߳����������ȼ�������PI

����һ�����ƣ����������ͨ�û�������ӦN�����ģ���Ҫʹ���̲߳�����ʵ��
���̴߳���N�������߳�
ÿ�������̼߳���һ�������񣬲����������
���̵߳ȴ�N�������߳����н����������и����̵߳Ľ���ۼ�
*/
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#define COUNT 1000
#define NUMBER 5
struct param{
	int start;
	int end;
}param;

struct result{
	float sum;
};

void *worker(void *arg){
	struct param *p = (struct param *)arg;
	struct result *result = (struct result *)malloc(sizeof(struct result));
	int i;
	double temp = 0;
	for(i=p->start;i<=p->end;i++){
		if(i % 2 == 0){
			temp -= (float)1/(2*i-1);
		}else{
			temp += (float)1/(2*i-1);
		}
	}
	result->sum = temp;
	return result;
}

int main(){
	pthread_t worker_tid[NUMBER];
	float sum = 0;
	int temp = COUNT/NUMBER;
	int i;
	for(i=1;i<=NUMBER;i++){
		struct param *p =  (struct param *)malloc(sizeof(struct param));
		p->start = temp*(i-1)+1;
		p->end = temp*i;
		pthread_create(&worker_tid[i-1],NULL,worker,p);
	}
	for(i=1;i<=NUMBER;i++){
		struct result *r;
		pthread_join(worker_tid[i-1],(void **)&r);
		sum += r->sum;
	}
	printf("result:%f\n",sum);
	return 0;
}
