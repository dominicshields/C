/* ftell example : getting size of a file */
#include <stdio.h>

int main (argc,argv)
int argc;
char *argv[];
{
  FILE * pFile;
  long size;

  pFile = fopen (argv[1],"rb");
  if (pFile==NULL) perror ("Error opening file");
  else
  {
    fseek (pFile, 0, SEEK_END);   // non-portable
    size=ftell (pFile);
    fclose (pFile);
    printf ("Size of myfile.txt: %ld bytes.\n",size);
  }
  return 0;
}