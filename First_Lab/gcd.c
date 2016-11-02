/*
Normal gcd 
================
a -> number 1
b -> number 2
r -> result
================
*/

#include <stdio.h>
#include <gmp.h>
#include <string.h>

mpz_t zero;
void gcd(mpz_t a, mpz_t b,mpz_t r)
{
	if(mpz_cmp_si(a,0) == 0)
	{
		mpz_set(r,b);
		return;
	}
	mpz_t t, x1,y1;
	mpz_init(t);
	mpz_mod(t,b,a);
	gcd(t,a,r);
}


int main(int argc,char **argv)
{
	mpz_t a,b,r,x,y;
	if(argc < 3)
	{
		printf(":Call : ./a.out <num1> <num2>");
		return 1;
	}

	mpz_init(r);
	mpz_init_set_str(a,argv[1],10);
	mpz_init_set_str(b,argv[2],10);
	mpz_init_set_str(zero,"0",10);
	gmp_printf("%Zd and %Zd",a,b);
	gcd(a,b,r);
	gmp_printf("\n\nResult: %Zd\n",r);
}