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

int main(int argc, char *argv[])
{
    if(argc != 3) {
	std::cout << "./tcp_srv ip port" << std::endl;
	return -1;
    }
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);
    TcpSocket sock;

    CHECK_RET(sock.Socket());
    CHECK_RET(sock.Bind(ip, port));
    CHECK_RET(sock.Listen());

    while(1) {
	TcpSocket client;
	if(sock.Accept(client) == false){
	    continue;
	}
	//创建多进程
	int pid = fork();
	if(pid < 0){
	    std::cerr << "pid fork error" << std::endl;	    
	    exit(2);
	}
	else if(0 == pid) {
	    //儿子进程创建子进程
	    pid_t pidd = fork();
	    if(pidd < 0){
		std::cerr << "ppid fork error" << std::endl;	    
		exit(3);
	    }
	    else if(0 == pidd){
		while(1) {
		    //孙子进程
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
	    //儿子进程
	    return 0;
	}
	//父进程
	wait(NULL);
	client.Close();
    }
    sock.Close();
    return 0;
}
