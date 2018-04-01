#include <stdio.h> 
#include<stdlib.h>
#include<sys/wait.h>
#include <sys/types.h>
#include <unistd.h> 
#include <signal.h>
#include <ctype.h> 
/* 允许建立的子进程个数最大值 */
#define MAX_CHILD_NUMBER 10 
/* 子进程睡眠时间 */
#define SLEEP_INTERVAL 2 
int proc_number=0; 
/* 子进程的自编号，从0开始 */
void do_something(); 

int main(int argc, char* argv[])
{   
    /* 子进程个数 */
    int child_proc_number = MAX_CHILD_NUMBER; 
    int i, ch;
    int status ;
    pid_t temp ;
    pid_t  child_pid; 
    pid_t pid[10]={0}; /* 存放每个子进程的id */ 
    if (argc > 1) /* 命令行参数第一个参数表示子进程个数*/ 
    {
        child_proc_number = atoi(argv[1]); 
        child_proc_number= (child_proc_number > 10) ? 10 :child_proc_number;
    }
    for (i=0; i< child_proc_number; i++) { 
        temp=fork() ;
        switch(temp)
        {
            case -1: printf("fork failed !! \n");     exit(-1);
            case 0:  proc_number = i ; do_something();   break ;
            default :pid[i]= temp ;     break ;
        }
        /* 填写代码，建立child_proc_number 个子进程要执行
        * proc_number = i; 
        * do_something(); 
        * 父进程把子进程的id保存到pid[i] */ 
    }
    /* 让用户选择杀死进程，数字表示杀死该进程，q退出 */
    while ((ch = getchar()) != 'q')  
    { 
        if (isdigit(ch)) 
        {
            kill(pid[ch-'0'],SIGTERM);
            /*  填写代码，向pid[ch-'0']发信号SIGTERM， 
            * 杀死该子进程 */ 
        }
    } 
    kill(0,SIGTERM);
    /* 在这里填写代码，杀死本组的所有进程 */ 
    return 0 ;
} 
void do_something() 
{ 
    for(;;) 
    {  
        printf("This is process No.%d and its pid is %d\n",proc_number,  getpid());
        sleep(SLEEP_INTERVAL); // 主动阻塞两秒钟
    }
}
