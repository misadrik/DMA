#include <stdio.h>
#include <sys/types.h>  /*提供类型pid_t,size_t的定义*/
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <assert.h>
#include <fcntl.h>

int main(int argc,char* const *argv)
{
	int fd;

	fd = open("/dev/testmodule", O_RDWR);
	if(fd<0)
	{
		printf("Open failed!\n");
		return -1;
	}

	unsigned long *addr = (unsigned long *) mmap((void*)0x42424000,0x1000,PROT_READ|PROT_WRITE, MAP_SHARED, fd,0x1000);
	
	assert(addr != MAP_FAILED);
	printf("mmap OK addr:%lx\n", addr);
	return 0;

}
