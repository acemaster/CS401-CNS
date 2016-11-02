/*
Extended gcd 
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
void extend_gcd(mpz_t a, mpz_t b, mpz_t r, mpz_t x, mpz_t y)
{
	// gmp_printf("\nOuter Entered: a: %Zd, b: %Zd zero: %Zd",a,b,zero);
	if(mpz_cmp_si(a,0) == 0)
	{
		// gmp_printf("\nEntered: a: %Zd, b: %Zd",a,b);
		mpz_set_si(x,0);
		mpz_set_si(y,1);
		mpz_set(r,b);
		return;
	}
	// printf("\nPassed if");
	mpz_t t, x1,y1;
	mpz_init(x1);
	mpz_init(y1);
	mpz_init(t);
	mpz_mod(t,b,a);
	// printf("\nCalling extended.....");
	extend_gcd(t,a,r,x1,y1);
	// gmp_printf("\nr: %Zd",r);
	mpz_fdiv_q(x,b,a);
	// gmp_printf("\n(b/a): %Zd",x);
	mpz_mul(x,x,x1);
	// gmp_printf("\n(b/a)*x1: %Zd",x);
	mpz_sub(x,x,y1);
	// gmp_printf("\n(b/a)*x1 - y1: %Zd",x);
	mpz_mul_si(x,x,-1);
	// gmp_printf("\ny1 - (b/a)*x1: %Zd",x);
	mpz_set(y,x1);
	// gmp_printf("\nEntered: a: %Zd, b: %Zd",a,b);
}


int main(int argc,char **argv)
{
	mpz_t a,b,r,x,y;
	long int a_sign=1,b_sign=1;
	if(argc < 3)
	{
		printf(":Call : ./a.out <num1> <num2>");
		return 1;
	}

	mpz_init(r);
	mpz_init(x);
	mpz_init(y);
	mpz_init_set_str(a,argv[1],10);
	mpz_init_set_str(b,argv[2],10);
	gmp_printf("%Zd and %Zd",a,b);
	if(mpz_sgn(a) == -1)
	{
		mpz_mul_si(a,a,-1);
		a_sign=-1;
	}
	if(mpz_sgn(b) == -1)
	{
		mpz_mul_si(b,b,-1);
		b_sign=-1;
	}
	mpz_init_set_str(zero,"0",10);
	extend_gcd(a,b,r,x,y);
	mpz_mul_si(x,x,a_sign);
	mpz_mul_si(y,y,b_sign);
	gmp_printf("\n\nResult: r: %Zd\nx: %Zd\ny: %Zd\n",r,x,y);
}