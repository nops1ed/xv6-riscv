#include "kernel/types.h"
#include "user/user.h"

int
main(int argc , char *argv[])
{
/*
  p = get a number from left neighbor
  print p
  loop:
    n = get a number from left neighbor
    if (p does not divide n)
        send n to right neighbor
*/
//feed numbers to pipeline
  int **pipeline = (int **)malloc(sizeof(int *) * 35);
  for (int i = 0 ; i < 35 ; i++)
    *(pipeline + i) = (int *)malloc(sizeof(int) * 2);
//initialize pipe cluster
  for (int i = 0 ; i < 35 ; i++)
    pipe((pipeline + i));
  //initialize pipeline 
  if (fork() == 0)
  {
      char *c = (char *)malloc(sizeof(char) * 2);
      *(c + 1) = '\0';
      for (int i = 0 ; i < 35 ; i++) 
      {
        *c = '0' + i;
        write(pipeline[0][1] , c , 2);
      }
      //close its input stream means the detection of next pipe should end
      close(pipeline[0][1]);
  }
  else
  {
    while ()
  }
}
