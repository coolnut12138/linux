### 我们在多进程的基础上，在子进程中再创建子进程，即孙子进程，然后立即将子进程退出，用孙子进程来处理收发数据。这样可以不用信号的自定义处理方式。但是这样和多进程的短板基本一样，所以还是多线程比较好。