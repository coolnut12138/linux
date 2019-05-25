/*使用udpsocket类完成udp客户端*/
#include "udpsocket.hpp"

int main(int argc, char *argv[])
{
    if(argc != 3) {
	cout << "输入参数不对，应该输入这三个参数：./udp_cli ip port" << endl;
	return -1;
    }

    string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    struct sockaddr_in srv_addr;
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(port);
    srv_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    UdpSocket sock;
    
    //1、创建套接字
    CHECK_RET(sock.Socket());

    //客户端不建议自己绑定地址，所以下一步是发送数据 
    while(1) {
	string buf;
	cout << "client say:";
	fflush(stdout);
	cin >> buf;
	sock.Send(buf, srv_addr);

	buf.clear();
	sock.Recv(buf);	//客户端对话的是服务端，所以不用传地址
	cout << "server say:" << buf << endl;
    }
    sock.Close();
    return 0;
}
