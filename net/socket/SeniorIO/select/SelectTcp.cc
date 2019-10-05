#include <iostream>
#include <sys/select.h>

int main()
{
    std::cout << sizeof(fd_set) << std::endl;
    return 0;
}

