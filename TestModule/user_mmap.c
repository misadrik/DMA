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

void write_to_mem(unsigned long *addr, unsigned int size, char* data);
void read_from_mem(unsigned long *addr, unsigned int size);

int main(int argc,char* const *argv)
{
	int fd;
	char *a = "abcdef";

	fd = open("/dev/testmodule", O_RDWR);
	if(fd<0)
	{
		printf("Open failed!\n");
		return -1;
	}

	unsigned long *addr = (unsigned long *) mmap((void*)0x42424000,0x1000,PROT_READ|PROT_WRITE, MAP_SHARED, fd,0x1000);
	
	assert(addr != MAP_FAILED);
	printf("mmap OK addr:%lx\n", addr);
	
	//write_to_mem(addr,6,*a);

	read_from_mem(addr,6);
	return 0;
	
}
void write_to_mem(unsigned long *addr, unsigned int size, char* data)
{
	int i = 0;
	for(i; i< size; i++)
	{
		*(addr+i) = data+i;
		//printf("addr: %lx, data: %c, wirte_data: %c\n", addr+i, *(addr+i), data+i);
	}
}
void read_from_mem(unsigned long *addr, unsigned int size)
{
	int i = 0;

	for(i = 0; i < size; i++)
	{
		printf("%c\n",*(addr+i));
	}
}
	


