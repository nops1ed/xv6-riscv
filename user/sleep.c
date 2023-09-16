//Prototype in system calls
//int sleep(int)

#include "kernel/types.h"
#include "user/user.h"

//remain old C style
int
main(int argc , char *argv[])
{
	if (argc < 2)
	{
		printf("find: nO arguemnts\n");
		exit(1);
	}
	sleep(atoi(argv[1]));
	exit(0);
}

