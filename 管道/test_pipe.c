/*************************************************************************
	> File Name: test_pipe.c
	> Author: 
	> Mail: 
	> Created Time: 2018年04月08日 星期日 22时59分40秒
 ************************************************************************/
//管道
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_DATA_LEN 256
#define DELAY_TIME 1

int main()
{
    pid_t pid;
    int pipe_fd[2];
    char buf[MAX_DATA_LEN];
    const char data[]="pipe Test Program";
    int real_read,real_write;
    memset((void*)buf,0,sizeof(buf));//为新申请的内存做初始化工作，buf的内容初始化为零
    //创建管道
    if(pipe(pipe_fd)<0)//成功，返回0，否则返回-1
    {
     printf("pipe create error\n");
     exit(1);
    }
    if((pid=fork())==0)
    {
    //子进程关闭写描述符，并通过使子进程暂停3秒等待父进程已关闭相应的读描述符
        close(pipe_fd[1]);//fd[0]:读管道，fd[1]:写管道
        sleep(DELAY_TIME*3);
        //子进程读取管道内容
        if((real_read=read(pipe_fd[0],buf,MAX_DATA_LEN))>0)//文件描述符，读完返回读取的字数，失败返回-1，buf：指定的缓冲区，指向一段内存单元；
        {printf("%d bytes read from the pipe is '%s'\n",real_read,buf);
        }
        //关闭子进程读描述
        close(pipe_fd[0]);
        exit(0);
    }
    else if (pid>0)
    {//父进程关闭读描述，并通过使父进程暂已关闭相应的写描述符停1秒等待子进程
        close(pipe_fd[0]);
        sleep(DELAY_TIME);
        if((real_write=write(pipe_fd[1],data,strlen(data)))!=-1)
            {
                printf("parent wrote %d byte :'%s'\n",real_write,data);
            }
        close(pipe_fd[1]);
        waitpid(pid,NULL,0);//pid>0时，只等待进程ID等于pid的子进程，不管其它已经有多少子进程运行结束退出了，只要指定的子进程还没有结束，waitpid就会一直等下去。
        exit(0);
    }
return 0;
}

