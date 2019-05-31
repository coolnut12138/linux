.PHONY:all
all:tcp_cli tcp_proce_orphan
tcp_proce_orphan:tcp_proce_orphan.cpp
	g++ $^ -o $@
tcp_cli:tcp_cli.cpp
	g++ $^ -o $@

.PHONY:clean
clean:
	rm tcp_cli tcp_proce_orphan
