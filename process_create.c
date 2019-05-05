/*封装fork/wait等操作,
 * 编写函数 process_create(pid_t* pid, void* func, void* arg), 
 * func回调函数就是子进程执行的入口函数, arg是传递给func回调函数的参数. */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

//函数指针
typedef void (*FUNC)();

void *func(void* arg)
{
    char* str = (char*)arg;
    printf("%s\n", str);
    printf("child pid = %d\n", getpid());
}

//创建进程
int process_create(pid_t* pid, void* func, char* arg)
{
    *pid = fork();
    if(*pid < 0){
	perror("fork error\n");
	return -1;
    }
    else if(*pid == 0){
	while(1){
	    //定义一个函数指针类型，指向那个函数
	    FUNC funct = (FUNC)func;
	    //传参传arg，不要传*arg，不然参数不匹配调不到回调函数
	    funct(arg);
	    sleep(1);
	}
    }
    else{
	pid_t ret = waitpid(-1, NULL, 0);
	printf("wait process success\n");
    }
}

int main()
{
    char* arg = "child is running!";
    pid_t pid;
    process_create(&pid, func, arg);
    return 0;
}


