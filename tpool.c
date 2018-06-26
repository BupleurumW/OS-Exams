#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <dirent.h>
#include <pthread.h>  
#include <assert.h>  
#include <sys/stat.h>
int count;
  
  
/* 
*�̳߳����������к͵ȴ���������һ��CThread_worker 
*�������������������������һ������ṹ 
*/  
typedef struct worker  
{  
    /*�ص���������������ʱ����ô˺�����ע��Ҳ��������������ʽ*/  
    void *(*listFolder) (char *arg);  
    char *arg;/*�ص������Ĳ���*/  
    struct worker *next;  
  
} CThread_worker;  


/*�̳߳ؽṹ*/  
typedef struct  
{  
    pthread_mutex_t queue_lock;  
    pthread_cond_t queue_ready;  
  
    /*����ṹ���̳߳������еȴ�����*/  
    CThread_worker *queue_head;  
  
    /*�Ƿ������̳߳�*/  
    int shutdown;  
    pthread_t *threadid;  
    /*�̳߳�������Ļ�߳���Ŀ*/  
    int max_thread_num;  
    /*��ǰ�ȴ����е�������Ŀ*/  
    int cur_queue_size;  
  
} CThread_pool;  
  
  //share resource  
static CThread_pool *pool = NULL;  
void* listFolder(char* dirname);
int pool_add_worker (void *(*listFolder) (char *arg), char *arg);  
void *thread_routine (void *arg);  
  
  
int isFolder(char* path)
{
	struct stat s_buf;
	stat(path,&s_buf);
	if(S_ISDIR(s_buf.st_mode))
		return true;
	return false;
}
void * getcount(char* filename)
{
	FILE *f;
	if((f=fopen(filename,"r"))==NULL)
	{
		printf("Cannot open file %s\n",filename);
	}
    char readline[1024];
    int count1=0;
    
    while (fgets(readline,1024, f)) //���ж�ȡ��ֱ������
    {
    	if(strstr(readline,"include"))
    	{
    		count1++;
		} 
    }
	fclose(f);
	count+=count1;
	printf("count: get: %d %d\n",count,count1);
//	return count;
}
char* concatPath(char*path,char*filename)
{
	char * tmp = (char*)malloc(strlen(path)+strlen(filename)+3);
	strcpy(tmp,path);
	strcat(tmp,"/");
	strcat(tmp,filename);

//	printf("concat %s %s\n",path,tmp);
	return tmp;
}
void* listFolder(char* dirname)
{
	int error;
    DIR *dir;
    struct dirent entry;
    struct dirent *result;
    struct stat buf;
    dir = opendir(dirname);
    //cout<<isFolder(dirname)<<endl;
 //   int count=0;
//	printf("ppppppppppppppppppppp%s\n",dirname);
    for (;;) {
    	printf("start \n");
        result = readdir(dir);

        if (result == NULL)
        {
        	printf("tttt NULL \n");
        	break;
		}
		else
		{
//			printf("%s\n",result->d_name);
		}
//        	printf("tttt NULL2 \n");
        
        int flag=0;
//        printf("\n,..,.,.,%s\n",result->d_name);
        char *tmp = concatPath(dirname,result->d_name);
//        printf("\ntmp : ,..,.,.,%s\n",tmp);
//        free(tmp);
		int len;
		if(isFolder(tmp)&&(strcmp(result->d_name, ".") != 0)&& (strcmp(result->d_name, "..") != 0))	
		{
	///		cout<<"is folder :"<<tmp<<endl;  
//			printf("is folder: %s\n",tmp);
			pool_add_worker(listFolder,tmp);
		}
		else
		{
		//	string a=result->d_name;
			int len = strlen(result->d_name);
			
	//		cout<<len<<endl;
			
//			printf("d_name len: %d\n",len);
			if(result->d_name[len-1]=='c'&&result->d_name[len-2]=='.')
			{
				flag=1;	
			}
    		if(flag==1)
			{
//				cout << "filepath" << readline<<" " <<readline.find("include")<< endl;
//				count++;
				
	//			cout<<"file :"<<a<<endl;  
//				printf("is folder2: %s\n",tmp);
				getcount(concatPath(dirname,result->d_name));
		 	} 
//		 		printf("is not folder: %s\n",tmp);
	//			count+=getcount(dirname+result->d_name);
		}

    }
    printf("+++++++++++++++count:  %d\n",count);
    closedir(dir);
   // return count;
}

void  
pool_init (int max_thread_num)  
{  
    pool = (CThread_pool *) malloc (sizeof (CThread_pool));  
  
    pthread_mutex_init (&(pool->queue_lock), NULL);  
    pthread_cond_init (&(pool->queue_ready), NULL);  
  
    pool->queue_head = NULL;  
  
    pool->max_thread_num = max_thread_num;  
    pool->cur_queue_size = 0;  
  
    pool->shutdown = 0;  
  
    pool->threadid = (pthread_t *) malloc (max_thread_num * sizeof (pthread_t));  
    int i = 0;  
    for (i = 0; i < max_thread_num; i++)  
    {   
        pthread_create (&(pool->threadid[i]), NULL, thread_routine,NULL);  
    }  
}  
  
  
  
/*���̳߳��м�������*/  
int  
pool_add_worker (void *(*listFolder) (char *arg), char *arg)  
{  
    /*����һ��������*/  
    CThread_worker *newworker = (CThread_worker *) malloc (sizeof (CThread_worker));  
    newworker->listFolder = listFolder;  
    newworker->arg = arg;  
    newworker->next = NULL;/*�����ÿ�*/  
  
    pthread_mutex_lock (&(pool->queue_lock));  
    /*��������뵽�ȴ�������*/  
    CThread_worker *member = pool->queue_head;  
    if (member != NULL)  
    {  
        while (member->next != NULL)  
            member = member->next;  
        member->next = newworker;  
    }  
    else  
    {  
        pool->queue_head = newworker;  
    }  
  
    assert (pool->queue_head != NULL);  
  
    pool->cur_queue_size++;  
    pthread_mutex_unlock (&(pool->queue_lock));  
    /*���ˣ��ȴ��������������ˣ�����һ���ȴ��̣߳� 
    ע����������̶߳���æµ�����û���κ�����*/  
    pthread_cond_signal (&(pool->queue_ready));  
    return 0;  
}  
  
  
  
/*�����̳߳أ��ȴ������е����񲻻��ٱ�ִ�У������������е��̻߳�һֱ 
����������������˳�*/  
int  
pool_destroy ()  
{  
    if (pool->shutdown)  
        return -1;/*��ֹ���ε���*/  
    pool->shutdown = 1;  
  
    /*�������еȴ��̣߳��̳߳�Ҫ������*/  
    pthread_cond_broadcast (&(pool->queue_ready));  
  
    /*�����ȴ��߳��˳�������ͳɽ�ʬ��*/  
    int i;  
    for (i = 0; i < pool->max_thread_num; i++)  
        pthread_join (pool->threadid[i], NULL);  
    free (pool->threadid);  
  
    /*���ٵȴ�����*/  
    CThread_worker *head = NULL;  
    while (pool->queue_head != NULL)  
    {  
        head = pool->queue_head;  
        pool->queue_head = pool->queue_head->next;  
        free (head);  
    }  
    /*���������ͻ�����Ҳ����������*/  
    pthread_mutex_destroy(&(pool->queue_lock));  
    pthread_cond_destroy(&(pool->queue_ready));  
      
    free (pool);  
    /*���ٺ�ָ���ÿ��Ǹ���ϰ��*/  
    pool=NULL;  
    return 0;  
}  
  
  
  
void *  
thread_routine (void *arg)  
{  
    printf ("starting thread 0x%x\n", pthread_self ());  
    while (1)  
    {  
        pthread_mutex_lock (&(pool->queue_lock));  
        /*����ȴ�����Ϊ0���Ҳ������̳߳أ���������״̬; ע�� 
        pthread_cond_wait��һ��ԭ�Ӳ������ȴ�ǰ����������Ѻ�����*/  
        while (pool->cur_queue_size == 0 && !pool->shutdown)  
        {  
            printf ("thread 0x%x is waiting\n", pthread_self ());  
            pthread_cond_wait (&(pool->queue_ready), &(pool->queue_lock));  
        }  
  
        /*�̳߳�Ҫ������*/  
        if (pool->shutdown)  
        {  
            /*����break,continue,return����ת��䣬ǧ��Ҫ�����Ƚ���*/  
            pthread_mutex_unlock (&(pool->queue_lock));  
            printf ("thread 0x%x will exit\n", pthread_self ());  
            pthread_exit (NULL);  
        }  
  
        printf ("thread 0x%x is starting to work\n", pthread_self ());  
  
        /*assert�ǵ��Եĺð���*/  
        assert (pool->cur_queue_size != 0);  
        assert (pool->queue_head != NULL);  
          
        /*�ȴ����г��ȼ�ȥ1����ȡ�������е�ͷԪ��*/  
        pool->cur_queue_size--;  
        CThread_worker *worker = pool->queue_head;  
        pool->queue_head = worker->next;  
        pthread_mutex_unlock (&(pool->queue_lock));  
  
        /*���ûص�������ִ������*/  
        (*(worker->listFolder)) (worker->arg);  
        free (worker);  
        worker = NULL;  
    }  
    /*��һ��Ӧ���ǲ��ɴ��*/  
    pthread_exit (NULL);  
}  
  
//    �����ǲ��Դ���  
  
void *  
myprocess (void *arg)  
{  
    printf ("threadid is 0x%x, working on task %d\n", pthread_self (),*(int *) arg);  
    sleep (1);/*��Ϣһ�룬�ӳ������ִ��ʱ��*/  
    return NULL;  
}  
  
int  
main (int argc, char **argv)  
{  
count=0;
    pool_init (3);/*�̳߳������������߳�*/  
      
    /*���������Ͷ��10������*/  
    int *workingnum = (int *) malloc (sizeof (int) * 10);  
    int i;  
    for (i = 0; i < 9; i++)  
    {  
        workingnum[i] = i;  
   //     pool_add_worker (listFolder, );  
    }  
    char a[]="C:/Users/Administrator/Desktop";
    char *b;
    b=a;
    pool_add_worker (listFolder,a); 
//    listFolder(a);
    /*�ȴ������������*/  
    sleep (5);  
    /*�����̳߳�*/  
    pool_destroy ();  
  
    free (workingnum);  
    return 0;  
}  
