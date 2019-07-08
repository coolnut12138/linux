#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main()
{
    int fd = open("./test.txt", O_RDWR | O_CREAT, 0664);
    if(fd < 0){
	perror("file open error");
	return -1;
    }

    int pid = fork();
    if(pid < 0){
	perror("fork error");
	return -1;
    }
    else if(pid == 0){
	printf("child pid is:%d\n", getpid());
	printf("parent pid is:%d\n", getppid());
	char *str = "i am child\n";
	write(fd, str, strlen(str));
	printf("child exited\n");
	exit(1);
    }
    printf("child pid is:%d\n", getpid());
    printf("parent pid is:%d\n", getppid());
    char *str = "i am father\n";
    write(fd, str, strlen(str));
    //wait(NULL);

    while(1){
	sleep(1);
	printf("i am father...\n");
    }
    return 0;
}
