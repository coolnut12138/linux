/*彩色进度条*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#define NONE "\e[0m"	//用于恢复默认的终端输出属性，否则会影响后续的输出
#define RED "\e[0;31m"
#define BLUE "\e[0;34m"
#define YELLOW "\e[0;33m"
#define PURPLE "\e[0;35m"

int main()
{
    char buf[100] = { 0 };
    size_t i = 0; 
    const char* label = "-/\\|";    //模拟转圈
    for(; i <= 20; ++i){
	buf[i] = '*';
	if(i <= 5)
	    printf(RED"%s[%d%%%c]\r"NONE,buf, i*5, label[i % 4]);
	else if(i > 5 && i <= 10)
	    printf(BLUE"%s[%d%%%c]\r"NONE,buf, i*5, label[i % 4]);
	else if(i > 10 && i <= 15)
	    printf(YELLOW"%s[%d%%%c]\r"NONE,buf, i*5, label[i % 4]);
	else if(i > 15 && i <= 20)
	    printf(PURPLE"%s[%d%%%c]\r"NONE,buf, i*5, label[i % 4]);
	fflush(stdout);
	usleep(100000);//单位是毫秒,让我们能够看到进度条的运行过程
	//sleep(1)单位是秒
    }
    printf("\n");
    return 0;
}
