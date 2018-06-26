#include <stdlib.h>
#include <stdio.h> 
#include <dirent.h>  
#include <string.h> 
#include<fcntl.h>
#include <iostream>  
using namespace std;  


int i=0; 
const char *delim =  ".";
DIR *dir = NULL; 
struct dirent *entry;
string file_list[1024];
string per_file_name="E:/a";
int count=0;

int is_dir(string s){
	if((dir = opendir(s.c_str()))==NULL) 
		return 0;
	else 
		return 1;
}



int build_list(DIR *dir)
{
	int count=0;
	while(entry=readdir(dir))  
    {   
        printf("filename%d = %s\n ",i,entry->d_name);  //输出文件或者目录的名称
        count++;
        per_file_name="E:/a/"+(string)entry->d_name;
       	if((is_dir(per_file_name)&&(strcmp(entry->d_name, ".") != 0)&& (strcmp(entry->d_name, "..") != 0)))
		{
		 	file_list[i] =  entry->d_name;
		 	printf("%s----\n",file_list[i].c_str());
			i++;   
		}
		//int end=i;
		for(int j=0;j<count;j++){
			//open_dir(entry,dir,"E:/a/"+entry->d_name);
        	//closedir(dir);    
    		 
		}
    }	
}

void open_dir(struct dirent *entry,DIR *dir,string f)
{
	int count=0;
 	build_list(dir);
 	//printf("%s----",file_list[2].c_str());
 	
} 
int main()  
{    
	string FilePath ="E:/a" ;
    int filesize = 0;  
 
    if((is_dir(FilePath)==0))
    {  
        printf("opendir failed!");  
        return -1;  
    }
    else  
    {    
		open_dir(entry,dir,FilePath);
        closedir(dir);    
    }  
    return 0;    
}
