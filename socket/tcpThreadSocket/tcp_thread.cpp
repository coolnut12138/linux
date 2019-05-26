/*通过封装的TcpSocket类实现多线程服务端程序*/
/*
 * 1、创建套接字
 * 2、绑定地址信息
 * 3、开始监听
 * 4、获取连接成功的客户端新建socket
 * 5、通过新建的客户端socket与客户端进行通信
 * 6、关闭套接字*/

//因为多进程还是很消耗资源的，当启动大量进程时，有可能会导致崩溃，所以使用多线程，轻量化

#include "tcpsocket.hpp"
#include <pthread.h>

void *thr_start(void *arg) {
    TcpSocket *client = (TcpSocket*)arg;
    while(1){
	//接收数据
	std::string buf;
	client->Recv(buf);
	std::cout << "client say:" << buf << std::endl;

	//发送数据
	std::cout << "server say:";
	fflush(stdout);
	std::cin >> buf;
	client->Send(buf);
    }
    delete client;
    return NULL;
}

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
	TcpSocket *client = new TcpSocket();	//局部变量会被释放导致接收不到
	if(sock.Accept(*client) == false){
	    //一个客户端接收失败不能导致整个程序退出，所以continue
	    continue;
	}

	pthread_t tid;
	pthread_create(&tid, NULL, thr_start, (void*)client);
	pthread_detach(tid);
    }
    sock.Close();
    return 0;
}
