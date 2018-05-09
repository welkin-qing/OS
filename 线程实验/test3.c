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
/*
　第一个参数为指向线程标识符的指针，第二个参数用来设置线程属性，第三个参数是线程运行函数的起始地址，最后一个参数是运行函数的参数。

实验过程 按照注释里的要求把代码补充完整，正确编译程序后，先预计一下这个程序的运行结果。具体的结果会是什么样？运行程序。开另一个终端窗口，运行“ps aux”命令，看看thread 的运行情况，注意查看thread 的CPU 占用率，并记录下这个结果。

回答下列问题 
1.你最初认为前三列数会相等吗？最后一列斜杠两边的数字是相等，还是大于或者小于关系？ 
相等，相等

2.最后的结果如你所料吗？有什么特点？对原因进行分析。 
不是。 每次的结果都很奇怪，出现了诡异的数据。 传参有问题想传进的数据跟实际传入的数据不一样。主线程速度很快，有可能传参，已经进入下一次for循环，导致传的参数发生了变化，甚至有可能已经进入do...while...导致参数被重新赋为0;可以通过传递值，传数组，申请动态数组来解决。

main_counter小于sum，main_counter++不是原子操作，子线程相互竞争main_counter，导致不能正常执行加1操作。可能当线程 1 还没完成加 1 操作的时候，此时，线程2 也开始执行 main_counter++（如果是单核CPU，线程 1 会暂时保存寄存器中的值，待下一个时间片到来时，恢复现场继续操作; 如果是多核CPU，线程 1 和线程 2 可能会同时执行++） ,但是线程 2 看到的main_counter 还是 0 ，所以线程 2完成了加 1 操作后，main_counter 还是 1。虽然两个线程各执行了一次加 1 操作，但是最终 main_counter 实际上只加了1次。这就导致main_counter 比理论值偏小。在3个蛮线程运行的情况下，理论值最大是实际值的3倍。

在子进程中进行加锁操作后，发现main_counter比sum要大，这时因为在求sum的时候，main_counter仍然在自增，归根到底还是竞争关系。在求sum时也要上锁，这样才能保证main_counter和sum能够同步。

3.thread 的CPU 占用率是多少？为什么会这样？

296%，开来三个子线程，cpu是多核，这个程序同时占用3个cpu

4.thread_worker()内是死循环，它是怎么退出的？你认为这样退出好吗？ 
在main函数中通过return来使函数退出主线程，则整个进程终止，此 时进程的所有线程也将终止。 不好，被动退出不如主动退出，被动退出可能会留下一些隐患。
*/
