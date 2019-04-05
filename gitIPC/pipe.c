/*匿名管道的使用*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
    //int pipe(int pipefd[2]);
    // pipefd:用于接收管道的操作句柄（文件描述符）
    // 返回值：成功：0，失败：-1
    int pipefd[2];
    int ret = pipe(pipefd);
    if(ret < 0)
    {
	perror("pipe error");
	return -1;
    }
    
    int pid = fork();
    if(pid < 0)
    {
	perror("fork error");
	exit(-1);
    }else if(pid == 0){
	close(pipefd[0]);
	//子进程写数据：helloworld~~
	int len = 0;
	while(1){
	    char *ptr = "helloworld~~";
	    printf("write len: %d\n", len);
	    int ret = write(pipefd[1], ptr, strlen(ptr));
	    len += ret;
	    printf("write len: %d\n", len);
	}
    }else{
	close(pipefd[1]);
	//父进程读数据，关闭写
	char buf[1024] = { 0 };
	int ret = read(pipefd[0], buf, 1023);
	printf("read buf:[%d][%s]\n", ret, buf);
    }
    while(1){
	printf("------parent");
	sleep(1);
    }
    return 0;
}
