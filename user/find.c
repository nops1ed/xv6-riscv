#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

static int __FOUND_ = 0;
//remain Old C style
char*
fmt(char *path)
{
   char *buf = (char *)malloc(sizeof(char) * (DIRSIZ + 1));
   char *p;
   
     // Find first character after last slash.
   for(p=path+strlen(path); p >= path && *p != '/'; p--)
       ;
    p++;
   
     // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  //memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

//find function should be recursive
//filename indicates the file we wanna find
void 
find (char *path , char *filename) 
{
  char buf[512] , *p;
  //initialize fd to tell whether open or stat filename successfully
  int fd;
  //describe current node stat
  struct stat st;
  struct dirent de;
  
  if ((fd = open(path , O_RDONLY)) < 0)
  {
    //send error message to standard error stream
    fprintf(2 , "find: cannot open %s\n" , path);
    return;
  }
  
  if (fstat(fd , &st) < 0)
  {
    fprintf(2 , "find: cannot stat %s\n" , path);
    close(fd);
    return ;
  }
  //You need to call find again & its para should be all files
  //Below current file
  if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf))  
  {
    printf("find: path too long\n");
    close(fd);
    return;
  }
  strcpy(buf , path);
  //locate the end of buffer
  p = buf + strlen(buf);
  *p++ = '/';
  //printf("Now the buffer is !%s!\n" , buf);
  while(read(fd , &de , sizeof(de)) == sizeof(de))
  {
    if (de.inum == 0)
        continue;
    memmove(p , de.name , DIRSIZ);
    p[DIRSIZ] = 0;
    if (stat(buf , &st) < 0)
    {
      printf("find: cannot stat %s\n" , buf);
      continue;
    }
    switch(st.type)
    {
      case T_FILE:
        //printf("Here we find some file %s\n" , fmt(buf));
        //printf("And Im finding file: !%s!\n" , filename);
        //printf("File find :%s! size = %d , filename_size = %d\n" , fmt(buf) , sizeof(fmt(buf)) , sizeof(filename));
        //printf("I wanna find !%s! , now we got !%s!\n" , filename, fmt(buf));
        //printf("The strcmp returns: %d\n" , strcmp(fmt(buf) , filename) );
        if (strcmp(filename , "") == 0 || strcmp(fmt(buf) , filename) == 0)
        //stupid gcc
        {
          __FOUND_ = 1;
          printf("%s\n" , buf);
        }
        continue;
      case T_DIR:
      {
        if (!strcmp(de.name , ".") || !strcmp(de.name , ".."))
          continue;
        //printf ("Now the buffer is !%s!\n" , buf);
        //char buf_new[64] = {"\0"};
        //printf("Now the buffer is %s\n" , buf);
        //strcpy(buf_new , buf);
        //memmove(buf_new + strlen(buf) , "/"  , 1);
        //memmove(buf_new + strlen(buf) + 1, de.name , strlen(de.name));
        //p = buf + sizeof(buf);
        //printf("p point to %c\n"  , *(p - 1));
        //*p++ = '/';
        //printf("And i pass %s\n" , buf_new);
        find(buf, filename);
      }
      case T_DEVICE:
    }
  }
  close(fd);
  return ;
}

int 
main(int argc ,char *argv[])
{
  if (argc < 2) 
  {
    find ("." , "");
    exit(0);
  }
  if (argc == 2)
    find("." , argv[1]);
  else
    find(argv[1] , argv[2]);
  if (!__FOUND_)
    printf("find: No such file or directory\n");
  exit(0);
}
