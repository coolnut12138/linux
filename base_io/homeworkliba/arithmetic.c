#include <stdio.h>
int add(int x, int y)
{
    return x + y;
}
int sub(int x, int y)
{
    return x - y;
}
int mul(int x, int y)
{
    return x * y;
}
int div(int x, int y)
{
    if(y != 0){
	return x / y;
    }else{
	printf("被除数不能为0\n");
	return -1;
    }
}
