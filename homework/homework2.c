#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/resource.h>

int main()
{
    int pid = getpid();
    int ppid = getppid();
    int fd = open("./test.txt", O_RDONLY);
    char buf[1024];
    int nice = getpriority(PRIO_PROCESS, 0);
    if(fd < 0){
	perror("open error");
    }
    printf("pid:%d\n", pid);
    printf("ppid:%d\n", ppid);
    printf("fd num:%d\n", fd);
    getcwd(buf, 1024);
    printf("当前路径：%s\n", buf);
    printf("nice:%d\n", nice);
    return 0;
}
