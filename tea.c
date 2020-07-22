/***************************************************************
tea.c - A command line version of The Tiny encryption algorithm
(TEA) which was developed by David Wheeler and Roger Needham of the
Cambridge Computer Laboratory.
Explanation is here
http://www.ftp.cl.cam.ac.uk/ftp/papers/djw-rmn/djw-rmn-tea.html

Usage:
to encrypt : tea e <input file> <encrypted file> <key>
to decrypt : tea d <encrypted file> <decrypted file> <key>

Dominic Shields May 2001 (encrypt and decrypt were lifted straight
from the examples the developers gave).
***************************************************************/
#include <stdio.h>
#include <stdlib.h>

FILE *infile;
FILE *outfile;

typedef struct teakey
{
unsigned long ka;
unsigned long kb;
unsigned long kc;
unsigned long kd;
}teakey;

teakey tk[4];

teakey * tkp[4]=
{
     &tk[0],
     &tk[1],
     &tk[2],
     &tk[3]
};
unsigned long bytecount=0;

/*--------------------------------------------------------------------*/
void encrypt(unsigned long text[])
{
        unsigned long y = text[0], z = text[1];
        unsigned long delta = 0x9e3779b9, sum = 0; int n;
     unsigned long * k=(unsigned long *) tkp[1];

     for (n= 0; n < 32; n++)
        {
                sum += delta;
                y += ((z << 4) + k[0]) ^ (z+sum) ^ ((z >> 5) + k[1]);
                z += ((y << 4) + k[2]) ^ (y+sum) ^ ((y >> 5) + k[3]);
        }
        text[0] = y;  text[1] = z;
}

/*--------------------------------------------------------------------*/
void decrypt(unsigned long text[])
{
        unsigned long y = text[0], z = text[1];
        unsigned long delta = 0x9e3779b9, sum = delta << 5;  int n;
     unsigned  long * k=(unsigned long *) tkp[1];

     for (n= 0; n < 32; n++)
        {
                z -= ((y << 4) + k[2]) ^ (y + sum) ^ ((y >> 5) + k[3]);
                y -= ((z << 4) + k[0]) ^ (z + sum) ^ ((z >> 5) + k[1]);
                sum -= delta;
        }
        text[0] = y; text[1] = z;
}

/*--------------------------------------------------------------------*/
void tea(char mode, FILE *infile, FILE *outfile)
{
char Text[8];
int i;

printf("mode is %c\n",mode);

while(!feof(infile))
{
     i = fread(Text, 1, 8, infile);          /* read 8 bytes from infile
into Text */
     if (i <= 0) break;
        while (i < 8)
        {
          Text[i++] = ' ';   /* pad last block with spaces */
        }

        switch (mode)
        {
                case 'e': encrypt((unsigned long*) Text); break;
                case 'd': decrypt((unsigned long*) Text); break;
        }
        fwrite(Text, 1, 8, outfile);          /* write 8 bytes from Text to
outfile */
          bytecount += 8;
}
}

/*--------------------------------------------------------------------*/

void inittea(unsigned long *keypntr,int index)
{

teakey* t=tkp[index];

t->ka=keypntr[0];
t->kb=keypntr[1];
t->kc=keypntr[2];
t->kd=keypntr[3];

}

/*--------------------------------------------------------------------*/
main(argc,argv)
int argc;
char *argv[];
{

if(argc < 5)
{
     printf("Error : usage is %s <mode> <input file> <output file> <key>\n",argv[0]);
     exit(1);
}

if ((infile = fopen(argv[2],"rb")) == NULL)
{
     printf("Error opening %s\n",argv[2]);
     exit(1);
}

if ((outfile = fopen(argv[3],"wb")) == NULL)
{
     printf("Error creating %s\n",argv[3]);
     exit(1);
}

inittea( (unsigned long *) argv[4],1);

tea(*argv[1],infile,outfile);

printf("%ld bytes written to %s\n",bytecount,argv[3]);

fclose(infile);
fclose(outfile);

return 0;
}