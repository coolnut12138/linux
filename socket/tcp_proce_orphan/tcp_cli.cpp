/*通过封装的TcpSocket类实现网络通信客户端*/
/*
 * 1、创建套接字
 * 2、向服务端发起连接请求
 * 3、收发数据
 * 4、关闭套接字*/

#include "tcpsocket.hpp"
#include <signal.h>

int main(int argc, char *argv[])
{
    if(argc != 3) {
	std::cout << "./tcp_cli ip port" << std::endl;
	return -1;
    }
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    TcpSocket sock;
    CHECK_RET(sock.Socket());
    CHECK_RET(sock.Connect(ip, port));

    while(1){
	std::string buf;
	std::cout << "client say:";
	fflush(stdout);
	std::cin >> buf;
	sock.Send(buf);

	buf.clear();
	sock.Recv(buf);
	std::cout << "server say:" << buf << std::endl;
    }
    sock.Close();
    return 0;
}
