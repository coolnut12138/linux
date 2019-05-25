/*使用udpsocket类完成udp服务端*/
#include "udpsocket.hpp"

int main(int argc, char *argv[])
{
    if(argc != 3) {
	cout << "输入参数不对，应该输入这三个参数：./udp_srv ip port" << endl;
	return -1;
    }

    string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    UdpSocket sock;
    
    //1、创建套接字
    CHECK_RET(sock.Socket());
    CHECK_RET(sock.Bind(ip, port));


    while(1) {
	//循环读取数据
	string buf;
	struct sockaddr_in cli_addr;
	sock.Recv(buf, &cli_addr);
	cout << "client say:" << buf.c_str() << endl;

	cout << "server say:";
	fflush(stdout);
	cin >> buf;
	sock.Send(buf, cli_addr);
    }
    sock.Close();
    return 0;
}
