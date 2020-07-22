

/*--------------------------------------------------------------------*/
void encrypt(unsigned long k[], unsigned long text[]) 
{
        unsigned long y = text[0], z = text[1];
        unsigned long delta = 0x9e3779b9, sum = 0; int n;
        for (n= 0; n < 32; n++)
        {
                sum += delta;
                y += ((z << 4) + k[0]) ^ (z+sum) ^ ((z >> 5) + k[1]);
                z += ((y << 4) + k[2]) ^ (y+sum) ^ ((y >> 5) + k[3]);
        }
        text[0] = y;  text[1] = z; 
}

/*--------------------------------------------------------------------*/
void decrypt(unsigned long k[], unsigned long text[]) 
{
        unsigned long y = text[0], z = text[1];
        unsigned long delta = 0x9e3779b9, sum = delta << 5;  int n;
        for (n= 0; n < 32; n++) 
        {
                z -= ((y << 4) + k[2]) ^ (y + sum) ^ ((y >> 5) + k[3]);
                y -= ((z << 4) + k[0]) ^ (z + sum) ^ ((z >> 5) + k[1]);
                sum -= delta;
        }
        text[0] = y; text[1] = z; 
}

/*--------------------------------------------------------------------*/
void tea(char mode, FILE *infile, FILE *outfile, unsigned long k[]) 
{
/* mode is 'e' for encrypt, 'd' for decrypt, k[] is the key.*/
char ch;
char Text[8];
int i;
while(!feof(infile))
{
	i = fread(Text, 1, 8, infile);          /* read 8 bytes from infile into Text */
	if (i <= 0) break;
        while (i < 8) 
        { 
        	Text[i++] = ' ';
        }                         /* pad last block with spaces */
        switch (mode)
        {
                case 'e': encrypt(k, (unsigned long*) Text); break;
                case 'd': decrypt(k, (unsigned long*) Text); break;
        }
        fwrite(Text, 1, 8, outfile);          /* write 8 bytes from Text to outfile */
}
}
