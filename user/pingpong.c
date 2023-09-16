#include "kernel/types.h"
#include "user/user.h"

//remain old C style

int
main (int argc , char *argv[])
{
	if (argc != 1)
	{
		printf("Too Much arguments\n");
		exit(1);
	}
//Here we allocate arrays to hold pipe fd
	//pipe from parent to child
  int *p1 = (int *)malloc(sizeof(int) * 2);
  pipe(p1);
	//pipe from child to parent
  int *p2 = (int *)malloc(sizeof(int) * 2);
	pipe(p2);
	if (fork() == 0)
	{
    close(p1[0]);
    close(p2[1]);
    char *buf = (char *)malloc(sizeof(char) * 32);
    *buf = '\0';
		//parent should bind its output stream to  pipe_1
		read (p2[0] , buf , 5);
    close(p2[0]);
		printf ("%d: received %s\n" , getpid() , buf);
		write (p1[1] , "pong", 5);
    close(p1[1]);
    free(buf);
		exit(0);
	}
	else {
    close(p1[1]);
    close(p2[0]);
    char *buf = (char *)malloc(sizeof(char) * 32);
    *buf = '\0';
		write (p2[1] , "ping", 5);
    close(p2[1]);
		read(p1[0], buf , 5);
    close(p1[0]);
		printf ("%d: recevied %s\n" , getpid() , buf);
    free(buf);
    exit(0);
	}
  free(p1);
  free(p2);
  return 0;
}
