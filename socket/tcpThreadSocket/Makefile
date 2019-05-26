all:tcp_cli tcp_thread
tcp_thread:tcp_thread.cpp
	g++ $^ -o $@ -lpthread
tcp_cli:tcp_cli.cpp
	g++ $^ -o $@
