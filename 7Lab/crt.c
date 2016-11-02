/*
CRT-> Chinese Remainder Theorem
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

void findinv(mpz_t a,mpz_t b,mpz_t x,mpz_t y)
{
	long int a_sign=1,b_sign=1;
	mpz_t r;
	mpz_init(r);
	gmp_printf("\n%Zd and %Zd",a,b);
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
	// mpz_abs(x,x);
	// mpz_abs(y,y);
	// gmp_printf("\nInverse Result: \nr: %Zd\nx: %Zd\ny: %Zd\n",r,x,y);
	mpz_mul_si(x,x,a_sign);
	mpz_mul_si(y,y,b_sign);
	if(mpz_sgn(y) == -1)
	{
		mpz_add(y,a,y);
	}
	gmp_printf("\nInverse Result: \nr: %Zd\nx: %Zd\ny: %Zd\n",r,x,y);
}
int main(int argc, char **argv)
{
	printf("==========CRT=================\n");
	printf("Please give arguments in the form: ./crt k a1 a2....ak m1 m2....mk ");
	int k;
	k=atoi(argv[1]);
	mpz_t X,x[10],a[10],m[10],M,A,z[10],y[10],Y;
	int i=0;
	for(i=0;i<k;i++)
	{
		mpz_init(m[i]);
		mpz_init(a[i]);
		mpz_init(y[i]);
		mpz_init(z[i]);
		mpz_init(x[i]);
	}
	mpz_init(M);
	mpz_init_set_str(M,"1",10);
	mpz_init(A);
	mpz_init(X);
	i=2;
	int temp=i;
	printf("%d\n",i);
	for(;i<k+temp;i++)
		mpz_init_set_str(a[i-temp],argv[i],10);
	temp=i;
	printf("%d\n",i);
	for(;i<k+temp;i++)
		mpz_init_set_str(m[i-temp],argv[i],10);
	for(i=0;i<k;i++)
		mpz_mul(M,M,m[i]);
	for(i=0;i<k;i++)
		gmp_printf("Result: a%d: %Zd\n",i,a[i]);
	for(i=0;i<k;i++)
		gmp_printf("Result: m%d: %Zd\n",i,m[i]);
	//calculate Zi
	for(i=0;i<k;i++)
		mpz_cdiv_q(z[i],M,m[i]);
	gmp_printf("\nResult: M: %Zd\n",M);
	for(i=0;i<k;i++)
		gmp_printf("Result: z%d: %Zd\n",i,z[i]);
	// //Calculate yi
	for(i=0;i<k;i++)
	{
		mpz_init(Y);
		findinv(z[i],m[i],Y,y[i]);
	}
	for(i=0;i<k;i++)	
		gmp_printf("Result: y%d: %Zd\n",i,y[i]);
	for(i=0;i<k;i++)
	{
		mpz_mul(x[i],a[i],m[i]);
		mpz_mul(x[i],x[i],y[i]);
	}
	for(i=0;i<k;i++)
	{
		gmp_printf("Result: x%d: %Zd\n",i,x[i]);
		mpz_add(X,X,x[i]);
	}
	gmp_printf("\n\nResult: X: %Zd\n",X);
	mpz_fdiv_r(A,X,M);
	gmp_printf("\n\nResult:\nM:%Zd\nx:%Zd\n",M,A);
	return 0;
}