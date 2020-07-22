/*
** compile with :
**  cc -O2 -o pi10 pi10.c -lgmp
**
******************************************
** gebruikte methode:
**   Srinivasa Ramanujan
**
** beginwaarden :
**    R = 2
**    A[0] = SQRT(2) - 1
**    Y[0] = SQRT(SQRT(2) - 1)
**
** reeks van berekeningen die iteratief doorlopen moeten worden :
**    Y[N+1] = (1-(1-Y[N]^4)^0.25)/(1+(1-Y[N]^4)^0.25)
**    A[N+1] = (1+Y[N+1])^4*A[N]-4^(N+1)*SQRT(R)*Y[N+1]*(1+Y[N+1]+Y[N+1]^2)
**
** het aantal juiste decimalen wordt ongeveer verviervoudigd per
** uitgevoerde stap.
**
*/

#include <stdio.h>
#include <gmp.h>
#include <sys/times.h>

main (int argc, char ** argv)
{
   MP_INT aold,
          yold,
          temp,
          temp2,
          temp3,
          mult,
          wortel;
   int i,
       stap = 0;
   unsigned long int ndigit,
                     vier = 1;
   long start,
        dif;
   struct tms tijden;
   
   times (&tijden);
   start = tijden.tms_utime;
   if (argc > 1)
   {
      ndigit = (unsigned long int) atoi (argv[1]);
   }
   else
   {
      ndigit = 40;
   }
   printf ("\n number of digits = %d\n\n", ndigit);
   ndigit = ndigit + 5;
   
   /* initialisatie */
   mpz_init_set_ui (&temp, 10);
   mpz_init (&mult);
   mpz_pow_ui (&mult, &temp, ndigit);

   mpz_init (&temp);
   mpz_init (&temp2);
   mpz_init (&temp3);
   mpz_pow_ui (&temp, &mult, 2);
   mpz_mul_ui (&temp2, &temp, 2);
   mpz_sqrt (&wortel, &temp2);
   
   mpz_init (&aold);
   mpz_sub (&aold, &wortel, &mult);

   mpz_init (&yold);
   mpz_mul (&temp, &aold, &mult);
   mpz_sqrt (&yold, &temp);
   
   times (&tijden);
   dif = tijden.tms_utime - start;
   printf (" na init : %ld\n", dif);
   do
   {
      mpz_pow_ui (&temp, &yold, 2);
      mpz_div (&temp2, &temp, &mult);
      mpz_pow_ui (&temp, &temp2, 2);
      mpz_div (&temp2, &temp, &mult);
      mpz_sub (&temp, &mult, &temp2);
      mpz_mul (&temp2, &temp, &mult);
      mpz_sqrt (&temp, &temp2);
      mpz_mul (&temp2, &temp, &mult);
      mpz_sqrt (&temp, &temp2);
      
      mpz_sub (&temp2, &mult, &temp);
      mpz_mul (&temp3, &temp2, &mult);
      
      mpz_add (&temp2, &mult, &temp);
      mpz_div (&yold, &temp3, &temp2); /* yold is ook y */

      times (&tijden);
      dif = tijden.tms_utime - start;
      printf (" na Y(N+1) : %ld\n", dif);
      
      mpz_pow_ui (&temp, &yold, 2);
      mpz_div (&temp2, &temp, &mult);
      mpz_add (&temp, &temp2, &yold);
      mpz_add (&temp2, &temp, &mult);
      mpz_mul (&temp, &temp2, &yold);
      mpz_div (&temp2, &temp, &mult);
      mpz_mul (&temp, &temp2, &wortel);
      mpz_div (&temp2, &temp, &mult);
      vier = vier * 4;
      mpz_mul_ui (&temp, &temp2, vier);
      
      mpz_add (&temp2, &mult, &yold);
      mpz_pow_ui (&temp3, &temp2, 2);
      mpz_div (&temp2, &temp3, &mult);
      mpz_pow_ui (&temp3, &temp2, 2);
      mpz_div (&temp2, &temp3, &mult);
      mpz_mul (&temp3, &temp2, &aold);
      mpz_div (&temp2, &temp3, &mult);
      
      mpz_sub (&aold, &temp2, &temp); /* this is the new A */
      
      stap++;
      times (&tijden);
      dif = tijden.tms_utime - start;
      printf (" na stap %d : %ld\n", stap, dif);
   } while (mpz_size (&yold) != 0);
      
   mpz_pow_ui (&temp, &mult, 2);
   mpz_div (&temp2, &temp, &aold);
   mpz_div_ui (&temp, &temp2, 100000);

   times (&tijden);
   dif = tijden.tms_utime - start;
   printf (" einde : %ld\n", dif);

   printvar (&temp, 1);
   
   mpz_clear (&aold);
   mpz_clear (&yold);
   mpz_clear (&temp);
   mpz_clear (&temp2);
   mpz_clear (&temp3);
   mpz_clear (&wortel);
   mpz_clear (&mult);
   return;
}

int printvar (MP_INT * variable, int pi)
{
   char * numberstr,
        * kar;
   int vijf,
       tien;

   numberstr = NULL;
   numberstr = mpz_get_str (numberstr, 10, variable);
   if (pi)
   {
      printf ("%c.\n", numberstr[0]);
      kar = numberstr + 1;
   }
   else
   {
      kar = numberstr;
   }
   vijf = 0;
   tien = 0;
   while (kar[0] != '\0')
   {
      putchar (kar[0]);
      tien++;
      if (tien == 10)
      {
         putchar (' ');
         tien = 0;
         vijf++;
         if (vijf == 5)
         {
            putchar ('\n');
            vijf = 0;
         }
      }
      kar++;
   }
   putchar ('\n');
   
   free (numberstr);

   return (0);
}