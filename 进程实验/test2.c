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
/*
kill()函数用于删除执行中的程序或者任务。调用格式为： kill(int PID, int IID)；
其中：PID是要被杀死的进程号，IID为向将被杀死的进程发送的中断号。

实验过程

先猜想一下这个程序的运行结果。假如运行“./process 20”，输出会是什么样？然后按照注释里的要求把代码补充完整，运行程序。开另一个终端窗口，运行“ps aux|grep process”命令，看看process 究竟启动了多少个进程。回到程序执行窗口，按“数字键+回车”尝试杀掉一两个进程，再到另一个窗口看进程状况。按q 退出程序再看进程情况。

回答下列问题 
1.你最初认为运行结果会怎么样？ 在屏幕上显示创建超过十个进程，并且循环输出。

2.实际的结果什么样？有什么特点？试对产生该现象的原因进行分析。 创建出十个进程，由于子进程进入死循环，设置sleep 2s，每隔2s就会输出一组数据，顺序不定。只有当输入q加上回车的时候，结束此程序。

如果是子进程，（1）退出循环，进入子进程的事件处理；（2）不退出循环，直接是子进程的事件处理，其中事件处理是一个循环，所在循环退出前，子进程是不会执行for语句的。 所以程序中，创建出的子进程一直处在死循环中，子进程不会再创建出新的进程。

输入数字来杀死一个进程出现僵尸进程，这时应该在杀死后用wait来等待。

3.proc_number 这个全局变量在各个子进程里的值相同吗？为什么？ 
不相同，因为各子进程间相互独立。

4.kill 命令在程序中使用了几次？每次的作用是什么？执行后的现象是什么？ 
2次，第一次为杀死指定序号的进程，第二次在程序运行结束时杀死所有进程。

5.使用kill命令可以在进程的外部杀死进程。进程怎样能主动退出？这两种退出方式哪种更好一些？
 通过return ，exit，_exit可以主动退出,主动退出更好。
*/
