//设置一个文件描述符为非阻塞
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

void SetNoBlock(int fd) {
    int fl = fcntl(fd, F_GETFL);    //fcntl中的F_GETFL获取到文件描述符
    if(fl < 0){
	std::cout << "fcntl error" << std::endl;
	return;
    }
    fcntl(fd, F_SETFL, fl | O_NONBLOCK);    //修改文件描述符属性为非阻塞O_NONBLOCK
}

int main()
{
    SetNoBlock(0);
    char buf[1024];
    while(1){
	ssize_t read_size = read(0, buf, sizeof(buf) - 1);
	if(read_size > 0) {
	    buf[read_size] = 0;
	    std::cout << buf << std::endl;
	}else{
	    std::cout << "default" << std::endl;
	    memset(buf, 0, 1024);
	}
	sleep(2);
    }
    return 0;
}
