#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

//remain old C style
int 
main(int argc , char *argv[])
{

  char *buf = (char *)malloc(sizeof(char ) * 32);
  //read from standard input stream
  int n;
  if (argc < 2)
  {
    while((n = read(0 , buf , sizeof(buf))) > 0)  
      write(1 , buf , n);
    exit(0);
  }
  char *args[MAXARG];
  for (int i = 1 ; i < argc ; i++)
    args[i - 1] = argv[i];
  int num_arg = argc - 1;
  char *p = buf;
  while(read(0 , p , 1) > 0)
  {
    if (*p == '\n')
    {
      *p = 0;
      if (fork() == 0)
      {
        args[num_arg] = buf;
        exec (args[0] , args);
        exit(0);
      }
      else
          wait(0);
      p = buf;
    }
    else 
      p++;
  }
  free(buf);
  exit(0);
}
