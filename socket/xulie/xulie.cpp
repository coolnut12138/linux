/*通过封装的TcpSocket类实现网络通信客户端*/
#include "tcpsocket.hpp"
#include <signal.h>

void sigcb(int no) {
    std::cout << "recv signo:" << no << std::endl;
}

int main(int argc, char *argv[])
{
    if(argc != 3) {
	std::cout << "./tcp_cli ip port" << std::endl;
	return -1;
    }
    signal(SIGPIPE, sigcb);
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    TcpSocket sock;
    CHECK_RET(sock.Socket());
    CHECK_RET(sock.Connect(ip, port));

    _person_info_t info;
    memset(&info, 0x00, sizeof(_person_info_t));
    strcpy(info.name,"lzl");
    strcpy(info.sex,"女");
    info.age = 18;
    send(sock.GetFd(), &info, sizeof(_person_info_t), 0);
    sock.Close();
    return 0;
}
