/*命名管道的基本使用*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
    //int mkfifio(const char *pathname, mode_t mode);
    //pathname:命名管道文件路径名
    //mode： 创建文件时的权限
    //返回值：成功：0	失败：-1
    umask(0);
    char *fifo = "./test.fifo";
    int ret = mkfifo(fifo, 0664);
    if(ret < 0){
	//出现错误，如果错误原因不是文件已存在，则报错退出
	if(errno != EEXIST){
	    perror("mkfifo error");
	    return -1;
	}
    }

    printf("ready to read\n");
    int fd = open(fifo, O_RDONLY);
    if(fd < 0){
	perror("open error");
	return -1;
    }
    printf("open fifo success!!!\n");
    while(1){
	char buf[1024] = {0};
	int ret = read(fd, buf, 1023);
	if(ret > 0){
	    printf("client say:[%s]\n", buf);
	}else if(ret == 0){
	    printf("write close!!\n");
	    return -1;
	}else{
	    printf("read error");
	    return -1;
	}
    }
    close(fd);

    return 0;
}
