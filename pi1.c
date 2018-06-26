/*
pi1.c: ʹ��2���̸߳����������ȼ�������PI

�������ȼ�����ʽ:  
���̴߳���1�������߳�
���̼߳��㼶����ǰ�벿��
�����̼߳��㼶���ĺ�벿��
���̵߳ȴ������߳����нY����,��ǰ�벿�ֺͺ�벿�����
*/
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#define N 1000

float worker_output = 0;

void *worker(void *arg){
	int i;
	for(i=N/2+1;i<=N;i++){
		if(i % 2 == 0){
			worker_output -= (float)1/(2*i-1);
		}else{
			worker_output += (float)1/(2*i-1);
		}
	}
}

int main(){
	pthread_t worker_tid;
	float result = 0;
	int i;
	for(i=1;i<=N/2;i++){
		if(i % 2 == 0){
			result -= (float)1/(2*i-1);
		}else{
			result += (float)1/(2*i-1);
		}
	}
	pthread_create(&worker_tid,NULL,worker,NULL);
	pthread_join(worker_tid,NULL);
	
	result += worker_output;
	printf("result:%f\n",result);
	return 0; 
}
