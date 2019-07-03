/*
 * 编写简易的minishell，实现能够运行shell的外部命令
 *	1、读取缓冲区
 *	2、解析输入
 *	3、创建子进程
 *	4、程序替换
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <ctype.h>

int main()
{
    char buf[1024] = {0};
    while(1) {
	printf("[wang@bogan]: ");
	fflush(stdout); //刷新标准输出缓冲区---将缓冲区的数据直接打印
	memset(buf, 0x00, 1024);
	// %[^\n] 的意思是从缓冲区中取数据，遇到\n为止，scanf原本的特性是遇到空格就停止
	// %*c 的意思是遇到\n停止，将\n后面的数据丢弃
	// %[^\n] scanf取数据的时候遇到各种空白字符就会停止读取
	//	  为了读取所有的输入，让scanf遇到换行的时候再终止
	// %*c	  scanf取数据之后，缓冲区中遗留换行符，取不出来，导致scanf非阻塞，
	//	  陷入死循环，所以取出一个字符并丢弃
	//scanf的返回值是读取数据的个数，如果读取失败，避免缓冲区中的换行取不出来，导致死循环（防备直接回车的情况）
	scanf("%[^\n]%*c", buf); 
	int pid = fork();
	if(pid < 0) {
	    perror("fork error");
	    return -1;
	}else if(pid == 0){
	    //ls -l >> abc.txt
	    char *str = buf;
	    int redirect_flag = 0; //'>'的数目,1是清空重定向，2是追加重定向
	    char *redirect_file = NULL;
	    int fd = -1;
	    while(*str != '\0') {
		//如果当前字符是重定向符号，则进入判断，将整个命令从这里截断
		//前面是命令，后面是重定向信息
		if (*str == '>') {
		    *str = '\0';
		    redirect_flag++;
		    if (*(str+1) == '>') {
			redirect_flag = 2;
		    }
		    str += redirect_flag;
		    //走完文件名前面的空白字符
		    while (isspace(*str) && *str != '\0') {
			*str++ = '\0';
		    }
		    //文件名的起始位置
		    redirect_file = str;
		    //走完文件名
		    while (!isspace(*str) && *str != '\0') {
			*str++;
		    }
		    //文件名最后需要结束符
		    *str = '\0';
		    continue;
		}
		str++;
	    }
	    if (redirect_flag == 1) {
		//清空重定向
		fd = open(redirect_file, O_CREAT | O_WRONLY | O_TRUNC, 0664);

		//这个重定向不能在父进程中完成，因为一旦父进程重定向了，shell程序后续的界面就打印不出来了，都会写入到这个文件中
		dup2(fd, 1);
	    }
	    else if (redirect_flag == 2) {
		//追加重定向
		fd = open(redirect_file, O_CREAT | O_WRONLY | O_APPEND, 0664);
		dup2(fd, 1);
	    }
	    // ls	    -l  -a
	    char *argv[32];
	    int argc = 0;
	    char *ptr = buf;
	    while(*ptr != '\0') {
		if (!isspace(*ptr)) {
		    argv[argc++] = ptr;
		    //将ls读完，指向空白字符处
		    while(*ptr != '\0' && !isspace(*ptr)) {
			ptr++;
		    }
		}
		//将空白字符全部替换为字符串结尾标志
		*ptr = '\0';
		ptr++;
	    }
	    //参数结尾必须为空，否则会解析命令失败
	    argv[argc] = NULL;
	    //子进程程序替换，运行指定命令
	    execvp(argv[0], argv);
	    exit(0);
	}
	//避免僵尸子进程
	waitpid(pid, NULL, 0);
    }
    return 0;
}
