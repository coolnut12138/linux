/*通过封装的TcpSocket类实现服务端程序*/
#include "tcpsocket.hpp"

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

    TcpSocket client;
    if(sock.Accept(client) == false){
    }

    _person_info_t person;
    recv(client.GetFd(), &person, sizeof(_person_info_t), 0);
    std::cout << " name:" << person.name << "sex:" << person.sex << "age:" << person.age << std::endl; 
    sock.Close();
    return 0;
}
