/************************************************
The Tiny Encryption Algorithm (TEA) by
David Wheeler and Roger Needham of the
Cambridge Computer Laboratory

**** ANSI C VERSION ****

Notes:
TEA is a Feistel cipher with XOR and
and addition as the non-linear mixing
functions.

Takes 64 bits of data in v[0] and v[1].
Takes 128 bits of key in k[0] - k[3].

TEA can be operated in any of the modes
of DES. Cipher Block Chaining is, for example,
simple to implement.

n is the number of iterations. 32 is ample,
16 is sufficient, as few as eight may be OK.
The algorithm achieves good dispersion after
six iterations. The iteration count can be
made variable if required.

Note this is optimised for 32-bit CPUs with
fast shift capabilities. It can very easily
be ported to assembly language on most CPUs.

delta is chosen to be the real part of (the
golden ratio Sqrt(5/4) - 1/2 ~ 0.618034
multiplied by 2^32).

************************************************/

typedef struct teakey
{
unsigned long ka;
unsigned long kb;
unsigned long kc;
unsigned long kd;
unsigned long rounds;
}teakey;


teakey tk[4];

teakey * tkp[4]=
{

	&tk[0],
	&tk[1],
	&tk[2],
	&tk[3]

};





void teaencipher(unsigned long *const v,int index)
{



unsigned  long * k=(unsigned long *) tkp[index];

teakey *tr=tkp[index];

register int n=tr->rounds;


register unsigned long       y=v[0],z=v[1],sum=0,delta=0x9E3779B9,
				a=k[0],b=k[1],c=k[2],d=k[3]; //,n=32;
printf("k[0] = %ld\n k1[1] = %ld\n k[2] = %ld\n k[3] = %ld\n",k[0],k[1],k[2],k[3]);
printf("tr = %ld\n",*tr);

   while(n-->0)
      {
      sum += delta;
      y += (z<<4)+a ^ z+sum ^ (z>>5)+b;
      z += (y<<4)+c ^ y+sum ^ (y>>5)+d;
      }

   v[0]=y; v[1]=z;
}




void teadecipher(unsigned long *const v,int index)
{
 
	unsigned  long * k=(unsigned long *) tkp[index];
	
teakey *tr=tkp[index];

register int n=tr->rounds;

	register unsigned long       y=v[0],z=v[1],sum, //=0xC6EF3720,
				delta=0x9E3779B9,a=k[0],b=k[1],c=k[2],
				d=k[3]; //,n=32;

   /* sum = delta<<5, in general sum = delta * n */

	sum=delta*n;

      while(n-->0)
      {
      z -= (y<<4)+c ^ y+sum ^ (y>>5)+d;
      y -= (z<<4)+a ^ z+sum ^ (z>>5)+b;
      sum -= delta;
      }
   
   v[0]=y; v[1]=z;
}




void inittea(unsigned long *keypntr,int index, int rounds)
{

teakey* t=tkp[index];

t->ka=keypntr[0];
t->kb=keypntr[1];
t->kc=keypntr[2];
t->kd=keypntr[3];
t->rounds=rounds;

printf("keypntr 0 = %ld\n",keypntr[0]);
printf("keypntr 1 = %ld\n",keypntr[1]);

}


void main(void)
{

	char key[]="abcdabcdabcdabcc";



	unsigned long buffer[4]=
	{
		0x12345678,0x9abcdef,
		0,1

	};

	
		inittea( (unsigned long *) &key,1,32);



	
	    teaencipher(&buffer[0],1) ;// (unsigned long *)key);
		teadecipher(&buffer[0],1);


	


}
