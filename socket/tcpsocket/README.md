## &emsp;&emsp;基于tcp协议实现的客户端服务器的通信，但是这种方式只能实现一条连接且只能收发消息一次，因为accept在新连接到来前一直时阻塞等待，所以，我们实现了多进程解决方案，多孤儿进程解决方案，多线程解决方案。
## &emsp;&emsp;当我们有新连接的时候，只需要调用一次accept即可，就不会造成只能收发一次数据。
