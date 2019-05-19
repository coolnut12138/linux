/*封装一个udp的套接字接口类
 * 实现：
 *	1、套接字创建
 *	2、绑定地址
 *	3、接收数据
 *	4、发送数据
 *	5、关闭套接字*/

#include <iostream>
#include <string>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;

class UdpSocket
{
public:
    UdpSocket(){
    }
    ~UdpSocket(){
    }
    bool Socket(){//创建套接字
	//int socket(int domain, int type, int protocol);
	//  domain:地址域
	//	AF_INET	    表示使用IPV4网络协议
	//  type:套接字类型
	//	SOCK_STREAM 流式套接字---默认协议TCP
	//	SOCK_DGRAM  数据报套接字-默认协议UDP
	//  protocol:
	//	0   使用默认协议
	//	IPPROTO_TCP 6	TCP协议
	//	IPPROTO_UDP 17	UDP协议
	//  返回值：套接字操作句柄-文件描述符	失败：-1
	_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);   //第三个参数是0的话，默认就是udp协议
	if(_sock < 0){
	    perror("socket error\n");
	    return false;
	}
	return true;
    }
    bool Bind(string &ip, uint16_t port){//为套接字绑定地址信息
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;  //指定地址域,此处是IPV4，还可以指定AF_INET6(IPV6),AF_LOCAL(本地)
	//因为网络通信使用大端字节序，因此端口和ip地址信息都需要被转换为网络字节序的数据
	//uint16_t htons(uint16_t hostshort);
	//  将16位的数据从主机字节序转换为网络字节序
	//uint32_t htonl(uint32_t hostshort);
	//  将32位的数据从主机字节序转换为网络字节序
	addr.sin_port = htons(port);	//指定端口
	//in_addr_t inet_addr(const char *cp);
	//将字符串点分十进制IP地址转换为网络字节序的整数IP地址
	addr.sin_addr.s_addr = inet_addr(ip.c_str());
    	//addr.sin_addr.s_addr = htonl(0xc0a87a84);	    //使用htonl将ip地址转换为网络字节序也可以 
	//inet_pton(AF_INET, ip.c_str(), &addr);	    //这样也可以
	
	//int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	//  sockfd: 创建套接字返回的套接字描述符
	//  addr：要绑定的地址信息
	//  addrlen：地址信息长度
	//  返回值：0	失败：-1
	socklen_t len = sizeof(struct sockaddr_in);
	int ret = bind(_sock, (struct sockaddr*)&addr, len);
	if(ret < 0) {
	    perror("bind error");
	    return false;
	}
	return true;
    }
    Recv(string &buf, sockaddr_in *_addr = NULL){//接收数据
	//ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
	//  sockfd:套接字描述符
	//  buf：用于保存接收的数据
	//  len：要接收的数据长度
	//  flags：默认0---阻塞接收---没有数据则阻塞
	//  src_addr：发送端的地址信息
	//  addrlen：输入输出型参数，用于获取地址长度
	//  返回值：实际接收的数据长度	失败：-1
	char tmp[1024] = {0};
	struct sockaddr_in addr;
	socklen_t len = sizeof(struct sockaddr_in);
	int ret = recvfrom(_sock, tmp, 1024, 0, (struct sockaddr*)&addr, &len);
	if(ret < 0){
	    perror("recvfrom error");
	    return false;
	}
	buf.assign(tmp, ret);	//从buf中获取指定长度的数据放入string对象中
	if(_addr != NULL) {
	    memcpy(_addr, &addr, len);
	}
	return true;
    }
    bool Send(string &buf, struct sockaddr_in &addr){//发送数据
	//ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,const struct sockaddr *dest_addr, socklen_t addrlen);
	// dest_addr: 目的端地址信息
	// addrlen：地址信息长度
	socklen_t len = sizeof(struct sockaddr_in);
	int ret = sendto(_sock, buf.c_str(), buf.size(), (struct sockaddr*)&addr, len);
	if(ret < 0){
	    perror("sendto error");
	    return false;
	}
	return true;
    }
    bool Close(){//关闭套接字
	close(_sock);
	_sock = -1;
	return true;
    }
private:
    int _sock;
};
