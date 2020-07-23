/* Word Count
** Harry M. Hardjono
*/

#include <stdio.h>
#include <ctype.h>

#define BUFSIZE  30000

main (int argc, char *argv[]) {
  int i,j;
  long int ltotal, wtotal, ctotal;
  long int lcount, wcount, ccount;
  FILE *infile;
  char buffer[BUFSIZE];
  char *head, *tail;
  int iss;                          /* in space */

  if (argc<2) {
    printf ("Usage: wc file1 file2 ...\n");
    goto PROGRAM_END;
  }
 
  ltotal = wtotal = ctotal = 0;
  for (i=1;i<argc;i++) {
    if ((infile=fopen(argv[i],"r")) == NULL) {
      fprintf (stderr, "unable to open file: %s\n", argv[i]);
      continue;
    }
 
    lcount = wcount = ccount = 0;
    iss = 1;
    while (j= fread(buffer, 1, BUFSIZE, infile)) 
    {
      ccount += j;
      for (head=&buffer[0], tail=&buffer[j]; head<tail; head++) 
      {
        if (iss && !isspace(*head)) 
        {
          ++wcount;
          iss = 0;
        } 
        else 
        {
          iss = isspace(*head);
          if (*head == '\n')
          {
          	 ++lcount;
          }
        }
      }
    }
 
    fclose(infile);
    printf ("%12ld %12ld %12ld %s\n", lcount, wcount, ccount, argv[i]);
    ltotal += lcount;  wtotal += wcount;  ctotal += ccount;
  }
 
  if (lcount!=ltotal || wcount!=wtotal || ccount!=ctotal)
    printf ("%12ld %12ld %12ld Total\n", ltotal, wtotal, ctotal);
 
  PROGRAM_END:
  return (0);
}
