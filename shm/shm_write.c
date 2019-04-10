/*共享内存的基本使用*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>

#define IPC_KEY 0x12345678
#define PROJ_ID 0x12345678
#define SHM_SIZE    4096
int main()
{
    //1、创建/打开共享内存
    //int shmget(key_t key, size_t size, int shmflg);
    //key_t是无符号数字
    //	key：共享内存标识符  这个key可以自己去指定成任意的数字也可以通过系统提供的接口 ftok 去生成一个System V IPC标准的 key 值
    //	size：共享内存大小
    //	shmflg：选项信息(权限信息以及如何操作共享内存)
    //	    IPC_CREAT   存在则打开，不存在则创建
    //	    IPC_EXCL	与IPC_CREAT同用，若存在则报错，不存在则创建
    //	    mode_flags	权限
    //	返回值：成功返回句柄：正整数     失败：-1
    //key_t key = ftok(".", PROJ_ID);
    int shmid = shmget(IPC_KEY, SHM_SIZE, IPC_CREAT|0664);
    if(shmid < 0){
	perror("shmget error");
	return -1;
    }
    //2、将共享内存映射到虚拟地址空间
    //void *shmat(int shmid, const void *shmaddr, int shmflg);
    //	shmid: 共享内存创建返回的句柄
    //	shmaddr：映射首地址-通常置NULL
    //	shmflg：
    //	    0	可读可写
    //	    SHM_RDONLY 只读
    //	返回值：成功：返回映射首地址    失败：返回 (void*)-1
    char *shm_start = (char*)shmat(shmid, NULL, 0);
    if(shm_start == (void*)-1){
	perror("shmat error");
	return -1;
    }
    //3、进行内存操作
    int i = 0;
    while(1){
	sprintf(shm_start, "开始输出！！！ +%d\n", i++);
	sleep(1);
    }
    //4、解除映射关系
    //int shmdt(const void *shmaddr);
    //	shmaddr:    映射首地址
    //	返回值：成功返回：0   失败返回：-1
    shmdt(shm_start);
    //5、删除共享内存
    //int shmctl(int shmid, int cmd, struct shmid_ds *buf);
    //	shmid:	句柄
    //	cmd： 操作
    //	    IPC_RMID   删除共享内存
    //	buf： 设置/获取属性信息
    //	    想获取传入首地址，不想获取了置NULL
    //	共享内存并不会被立即删除，而是判断当前连接数，若不为0，则拒绝后续连接，直到连接数为0的时候才删除这块共享内存。
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
