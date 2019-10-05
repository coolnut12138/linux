#pragma once

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

class Server{
private:
    int listen_sock;
    int port;
public:
    Server(const int& port_):port(port_)
    {}
    
    void InitServer()
    {
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
	    std::cerr << socket error << std::endl;
	    exit(2);
	}

	int opt = 1;
	setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = htonl(INADDR_ANY);

	int ret = bind(listen_sock, (struct sockaddr*)&local, sizeof(local));
	if(ret < 0){
	    std::cerr << "bind error" << std::endl;
	    exit(3);
	}

	ret = listen(listen_sock, 5);
	if(ret < 0){
	    std::cerr << "listen error" << std::endl;
	    exit(4);
	}

    }

    void Start()
    {
	
    }

    ~Server()
    {}
};


