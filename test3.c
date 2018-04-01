/*************************************************************************
	> File Name: test3.c
	> Author: 
	> Mail: 
	> Created Time: 2018年04月01日 星期日 15时56分07秒
 ************************************************************************/

#include<stdio.h>

/*  POSIX 下线程控制的实验程序残缺版 */ 
#include <stdio.h> 
#include <sys/types.h>
#include <unistd.h> 
#include <ctype.h>
#include <pthread.h> 
pthread_mutex_t mutex ;
#define MAX_THREAD 3 /* 线程的个数 */
unsigned long long main_counter, counter[MAX_THREAD]; 
/* unsigned long  long是比long还长的整数 */
void* thread_worker(void*); 
int main(int argc,char* argv[])
{     
    int i, rtn, ch;     
    pthread_t pthread_id[MAX_THREAD] = {0}; /* 存放线程id*/ 
    for (i=0; i<MAX_THREAD; i++)
    {       
        pthread_create(&pthread_id[i] ,NULL ,thread_worker ,(void *)&i);
        /* 在这里填写代码，用pthread_create建一个普通的线程，
        线程id存入pthread_id[i]，线程执行函数是thread_worker
        并i作为参数传递给线程 */
    }    
    do
    {
        /* 用户按一次回车执行下面的循环体一次。按q退出 */          
        unsigned long long sum = 0;    
        /* 求所有线程的counter的和 */
      pthread_mutex_lock(&mutex);
        for (i=0; i<MAX_THREAD; i++) 
        {
            /* 求所有counter的和 */         
            sum += counter[i];             
            printf("counter  == %llu ", counter[i]);        
        }
        printf("main_counter == %llu/sum == %llu", main_counter, sum);   
      pthread_mutex_unlock(&mutex);
    }while ((ch = getchar()) != 'q'); 
    pthread_mutex_destroy(&mutex) ; //清除锁
    return 0;
} 
void* thread_worker(void* p)
{   
    int thread_num;  
    thread_num=  *(int *)p ;
    /* 在这里填写代码，把main中的i的值传递给thread_num */ 
    for(;;) 
    { /* 无限循环 */      
    pthread_mutex_lock(&mutex);
     counter[thread_num]++; /* 本线程的counter加一 */ 
     main_counter++;    /* 主counter 加一 */   
    pthread_mutex_unlock(&mutex);
    } 
}
