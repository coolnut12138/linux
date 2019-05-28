/*基于封装的TcpSocket类实现一个http服务端程序*/
/*传输层依然还是一个tcp服务端；不过应用层解析的还是http协议数据*/

#include "tcpsocket.hpp"
#include <sstream>

int main(int argc, char *argv[])
{
    if(argc != 3){
	std::cout << "输入参数不对！请重新输入：./httpserver ip port" << std::endl;
	return -1;
    }
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);
    TcpSocket sock;
    CHECK_RET(sock.Socket());
    CHECK_RET(sock.Bind(ip, port));
    CHECK_RET(sock.Listen());

    while(1) {
	TcpSocket clisock;
	if(sock.Accept(clisock) == false) {
	    continue;
	}
	std::string req;
	clisock.Recv(req);
	std::cout << "req:[" << req << "]" << std::endl;

	std::string body = "<html><head><meta http-equiv='content-type' content='text/html;charset=utf-8'></head><h1>hello</h1></html>";
	std::stringstream resp;
	resp << "HTTP/1.1 200 OK\r\n";
	resp << "Content-Length: ";
	resp << body.length() << "\r\n\r\n";

	std::string header = resp.str();
	clisock.Send(header);
	clisock.Send(body);
	clisock.Close();
    }
    sock.Close();
    return 0;
}
