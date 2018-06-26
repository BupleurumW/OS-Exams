/*
sort.c: 多线程排序

主线程创建一个辅助线程
主线程使用选择排序算法对数组的前半部分排序
辅助线程使用选择排序算法对数组的后半部分排序
主线程等待辅助线程运行結束后,使用归并排序算法归并数组的前半部分和后半部分
*/
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

struct param{
	int start;
	int end;
	int array[100];
};

void *worker(void *arg){
	struct param *p = (struct param *)arg;
	int i;
	for(i=p->start;i<p->end;i++){
		int temp = i,j;
		for(j=i+1;j<=p->end;j++){
			if(p->array[j] < p->array[temp])	temp = j;
		}
		int t = p->array[temp];
		p->array[temp] = p->array[i];
		p->array[i] = t;
	}
}

int main(){
	pthread_t worker_tid;
	struct param *p = (struct param *)malloc(sizeof(struct param));
	int n,i;
	printf("enter the number of array:");
	scanf("%d",&n);
	printf("enter array:");
	for(i=0;i<n;i++){
		scanf("%d",&p->array[i]);
	}
	for(i=0;i<n/2 - 1;i++){
		int temp = i,j;
		for(j=i+1;j<n/2;j++){
			if(p->array[j] < p->array[temp])	temp = j;
		}
		int t = p->array[temp];
		p->array[temp] = p->array[i];
		p->array[i] = t;
	}
	p->start = n/2;
	p->end = n - 1;
	pthread_create(&worker_tid,NULL,worker,p);
	pthread_join(worker_tid,NULL);
	//归并前后两部分
	int j;
	i=0,j=n/2;
	while(j<n && i<j){
		if(p->array[j] < p->array[i]){
			int temp = p->array[j];
			for(int k=j;k>=i+1;k--){
				p->array[k] = p->array[k-1];
			}
			p->array[i] = temp;
			i++;
			j++;
		}else{
			i++;
		}
	}
	//print 
	printf("\nresult:\n");
	for(i=0;i<n;i++){
		printf("%d ",p->array[i]);
	}
	return 0;
}
