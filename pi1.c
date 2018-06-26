/*
pi1.c: 使用2个线程根据莱布尼兹级数计算PI

莱布尼兹级数公式:  
主线程创建1个辅助线程
主线程计算级数的前半部分
辅助线程计算级数的后半部分
主线程等待辅助线程运行結束后,将前半部分和后半部分相加
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
