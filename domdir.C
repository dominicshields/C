#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h> 

struct stat buf;
struct tm *tm_ptr;
int i;
unsigned long t;
char buffer[256];

/*
struct stat {                                               
    dev_t     st_dev;      ID of device containing file 
    ino_t     st_ino;      inode number               
    mode_t    st_mode;     protection                  
    nlink_t   st_nlink;    number of hard links        
    uid_t     st_uid;      user ID of owner            
    gid_t     st_gid;      group ID of owner           
    dev_t     st_rdev;     device ID (if special file) 
    off_t     st_size;     total size, in bytes        
    blksize_t st_blksize;  blocksize for filesystem I/O 
    blkcnt_t  st_blocks;   number of blocks allocated   
    time_t    st_atime;    time of last access         
    time_t    st_mtime;    time of last modification  
    time_t    st_ctime;    time of last status change 
}; 
*/

main(argc,argv)
int argc;
char *argv[];
{
   if(argc < 2)
   {
     fprintf(stderr,"Error, usage is domdir <inputfile>\n");
      exit(1);
   }
   i = stat(argv[1],&buf);
   if(i < 0)
   {
     fprintf(stderr,"Error, inputfile %s does not exist\n",argv[1]);
      exit(1);
   }
   t = buf.st_mtime;
   tm_ptr = localtime(&t);
   i = strftime(buffer,256,"Date: %A %d %B %Y Time: %H:%M:%S%z", tm_ptr);
   fprintf(stdout,"%s %s\n",argv[1],buffer);
   return 0;
 }
