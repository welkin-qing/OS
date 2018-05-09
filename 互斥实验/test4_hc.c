/*************************************************************************
	> File Name: test4_hc.c
	> Author: 
	> Mail: 
	> Created Time: 2018年04月12日 星期四 17时19分22秒
 ************************************************************************/

#include<stdio.h>
//* POSIX 下线程死锁的演示程序 */ 

//修改为正确的版本
#include <stdio.h> 
#include <sys/types.h>
#include <unistd.h> 
#include <ctype.h>
#include <pthread.h> 

#define LOOP_TIMES 10000 

/*用宏PTHREAD_MUTEX_INITIALIZER来初始化 */
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void* thread_worker(void*);
void critical_section(int thread_num, int i); 

int main(void) 
{     
    int rtn, i;     
    pthread_t pthread_id = 0; /* 存放子线程的id */ 
    rtn = pthread_create(&pthread_id,NULL, thread_worker, NULL ); 
    if(rtn != 0)
    {            
        printf("pthread_create ERROR!\n"); 
        return -1; 
    } 
    for (i=0; i<LOOP_TIMES; i++) 
    { 
        pthread_mutex_lock(&mutex2);
        pthread_mutex_lock(&mutex1); 
        critical_section(1, i); 
        pthread_mutex_unlock(&mutex2);
        pthread_mutex_unlock(&mutex1);
    } 

    pthread_mutex_destroy(&mutex1); 
    pthread_mutex_destroy(&mutex2); 
    return 0;
} 
void* thread_worker(void* p) 
{ 
    int i; 
    for (i=0; i<LOOP_TIMES; i++)
    { 
        pthread_mutex_lock(&mutex2);
        pthread_mutex_lock(&mutex1); 
        critical_section(2, i); 
        pthread_mutex_unlock(&mutex2);
        pthread_mutex_unlock(&mutex1);
    }
} 
void critical_section(int thread_num, int i) 
{ 
    printf("Thread%d: %d\n", thread_num,i);
}

/*
实验一： 找到thread.c 的代码临界区，用临界区解决main_counter 与sum 不同步的问题。

实验二： 仔细阅读程序，编译程序后，先预计一下这个程序的运行结果。运行程序。若程序没有响应，按ctrl+c 中断程序运行，然后再重新运行，如此反复若干次，记录下每次的运行结果。若产生了死锁，请修改程序，使其不会死锁。

回答下列问题

    你预想deadlock.c 的运行结果会如何？ 在某一时刻形成死锁

    deadlock.c 的实际运行结果如何？多次运行每次的现象都一样吗？为什么会这样？ 在某一时刻形成死锁，但是每次形成死锁的位置不同。 因为这里形成死锁的原因是：某一时刻，线程1上了A锁，线程2上了B锁，但是这时线程1要上B锁，线程2要上A;两个线程就会在无线等待，形成死锁。 要将两个线程上锁的顺序保持一致。

*/
