/*************************************************************************
	> File Name: test1.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年04月01日 星期日 13时52分07秒
 ************************************************************************/

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
int main(void)
{
    int i;
    for(i=0;i<2;i++)
    {
        fork();
        printf("-");
    }
    wait(NULL);
    wait(NULL);
    return 0;
}
