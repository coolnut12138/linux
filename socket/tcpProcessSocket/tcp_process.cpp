/*通过封装的TcpSocket类实现多进程服务端程序*/
/*
 * 1、创建套接字
 * 2、绑定地址信息
 * 3、开始监听
 * 4、获取连接成功的客户端新建socket
 * 5、通过新建的客户端socket与客户端进行通信
 * 6、关闭套接字*/
#include "tcpsocket.hpp"
#include <signal.h>
#include <sys/wait.h>

void sigcb(int no) {
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

int main(int argc, char *argv[])
{
    if(argc != 3) {
	std::cout << "./tcp_srv ip port" << std::endl;
	return -1;
    }
    signal(SIGCHLD, sigcb); //更改信号的处理方式
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);
    TcpSocket sock;

    CHECK_RET(sock.Socket());
    CHECK_RET(sock.Bind(ip, port));
    CHECK_RET(sock.Listen());

    while(1) {
	TcpSocket client;
	//程序卡在accept是因为用户无法获知客户端的新连接什么时候到来
	//如果能知道什么时候到来就不会阻塞；只需要在来的时候调用一次就行。
	//这样一个客户端进行一个来回就会阻塞，所以要使用多线程
	if(sock.Accept(client) == false){
	    //一个客户端接收失败不能导致整个程序退出，所以continue
	    continue;
	}
	//创建多进程
	int pid = fork();
	if(0 == pid) {
	    while(1) {
	    //子进程
		//接收数据
		std::string buf;
		client.Recv(buf);
		std::cout << "client say:" << buf << std::endl;

		//发送数据
		std::cout << "server say:";
		fflush(stdout);
		std::cin >> buf;
		client.Send(buf);
	    }
	}
	//父进程
	client.Close();
	//本来父进程是需要在这等待子进程退出的，不等待会僵尸进程，但是如果等待程序又会阻塞，所以在上面改信号的处理方式。	
    }
    sock.Close();
    return 0;
}
