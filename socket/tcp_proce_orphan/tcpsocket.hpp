/*封装一个TcpSocket类，向外提供轻便的socket接口*/
/* 1、创建套接字    
 * 2、为套接字绑定地址
 * 3.1、客户端向服务端发起连接请求
 * 3.2、服务端开始监听
 * 4、服务端获取一个已经连接成功客户端的新建socket
 *
 * 5、客户端与服务端收发数据
 * 6、关闭套接字*/
#include <iostream>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define CHECK_RET(q) if((q) == false) {return -1;}

class TcpSocket
{
public:
    TcpSocket()
	:_sockfd(-1)
    {}
    ~TcpSocket()
    {}
    bool Socket() {
	_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(_sockfd < 0) {
	    std::cout << "socket error" << std::endl;
	    return false;
	}
	return true;
    }
    bool Bind(std::string &ip, uint16_t port) {
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip.c_str());
	socklen_t len = sizeof(struct sockaddr_in);

	int ret = bind(_sockfd, (struct sockaddr*)&addr, len);
	if(ret < 0) {
	    std::cout << "bind error" << std::endl;
	    return false;
	}
	return true;
    }
    //开始监听，设置服务端的同一时间最大并发连接数
    bool Listen(int baklog = 5) {
	//int listen(int sockfd, int backlog);
	//  sockfd: 套接字描述符
	//  backlog：backlog设置内核中已完成连接队列的最大结点数量
	int ret = listen(_sockfd, baklog);
	if(ret < 0) {
	    std::cout << "listen error" << std::endl;
	    return false;
	}
	return true;
    }
    //获取连接成功客户端socket，并且返回客户端的地址信息，形参中的sock。
    bool Accept(TcpSocket &sock, struct sockaddr_in *addr = NULL) {
	//int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
	//  sockfd: 套接字描述符
	//  addr：  输出型参数：用于存储客户端地址信息
	//  addrlen：输入输出型参数：用于设置想要的地址长度和保存实际的地址长度
	//  返回值：返回为客户端连接新建的socket描述符，    失败：-1
	//  接下来与客户端的通信都是通过这个socket描述符实现的
	struct sockaddr_in _addr;
	socklen_t len = sizeof(struct sockaddr_in);
	int newfd = accept(_sockfd, (struct sockaddr*)&_addr, &len);
	if(newfd < 0) {
	    std::cout << "accept error" << std::endl;
	    return false;
	}
	sock.SetFd(newfd);
	if(addr != NULL) {
	    memcpy(addr, &_addr, len);
	}
	return true;
    }
    //客户端向服务端发起连ddrlen接请求
    bool Connect(std::string& ip, uint16_t port) {
	//int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	//  sockfd: 套接字描述符
	//  addr：  服务端监听地址信息
	//  addrlen：地址长度信息
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip.c_str());
	socklen_t len = sizeof(struct sockaddr_in);
	int ret = connect(_sockfd, (struct sockaddr*)&addr, len);
	if(ret < 0) {
	    std::cout << "connect error" << std::endl;
	    return false;
	}
	return true;
    }
    //通信接口:tcp服务端也可以直接先发送数据(因为连接已经建立成功了)
    //返回实际接收的长度
    bool Recv(std::string &buf) {
	//ssize_t recv(int sockfd, void *buf, size_t len, int flags);
	//  sockfd: 套接字描述符
	//  buf：   存储接受的数据
	//  len：   想要接受的长度
	//  flags： 
	//	    0--默认阻塞  
	//	    MSG_PEEK--接收数据但是不从接收缓冲区移除
	// 返回值：
	//	    成功--返回实际接收的字节数
	//	    0--当对端关闭时返回0
	//	    -1 -- 错误返回-1
	char tmp[1024] = {0};
	int ret = recv(_sockfd, tmp, 1024, 0);
	if(ret == 0) {
	    std::cout << "peer shutdown" << std::endl;
	    return false;
	}else if(ret < 0) {
	    std::cout << "recv error" << std::endl;
	    return false;
	}
	buf.assign(tmp, ret);
	return true;
    }
    bool Send(std::string &buf) {
	//ssize_t send(int sockfd, const void *buf, size_t len, int flags);
	//  返回值：实际发送的字节数	失败：-1
	//  若连接已经断开，发送会触发异常导致程序退出
	int ret = send(_sockfd, buf.c_str(), buf.size(), 0);
	if(ret < 0) {
	    std::cout << "send error" << std::endl;
	    return false;
	}
	return true;
    }
    bool Close() {
	close(_sockfd);
	_sockfd = -1;
	return true;
    }
    void SetFd(int sockfd) {
	_sockfd = sockfd;
    }
private:
    int _sockfd;
};
