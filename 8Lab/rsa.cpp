/*
RSA
================
a -> number 1
b -> number 2
r -> result
================
*/
#include <bits/stdc++.h>
#include <stdio.h>
#include <gmp.h>
#include <string.h>

using namespace std;

mpz_t zero,one;
gmp_randstate_t state;

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

void findprime(mpz_t phin,mpz_t prime)
{
	mpz_init(prime);
	long int phinint;
	mpz_t gcdget;
	mpz_init(gcdget);
	phinint = mpz_get_ui(phin);
	int k=0;
	while(1)
	{
		mpz_urandomm(prime,state,phin);
		gmp_printf("Possible prime: %Zd\n",prime);
		mpz_gcd(gcdget,prime,phin);
		if(mpz_cmp(gcdget,one)==0)
			break;
	}
}

void printgmp(char *str,mpz_t p)
{
	cout<<str;
	gmp_printf(": %Zd\n",p);
}

int main(int argc, char **argv)
{
	gmp_randinit_default(state);
	printf("==========RSA=================\n");
	printf("Please give arguments in the form: ./rsa p q m");
	mpz_t p,q,pneg,qneg;
	mpz_init_set_str(p,argv[1],10);
	mpz_init_set_str(q,argv[2],10);
	mpz_init_set_str(one,"1",10);
	mpz_t n,phin;
	mpz_init(n);
	mpz_init(phin);
	mpz_init(pneg);
	mpz_init(qneg);
	mpz_mul(n,p,q);
	mpz_sub(pneg,p,one);
	mpz_sub(qneg,q,one);
	mpz_mul(phin,pneg,qneg);
	mpz_t e;
	mpz_t d,y;
	do
	{
		findprime(phin,e);
		mpz_init(d);
		mpz_init(y);
		findinv(e,phin,d,y);
		gmp_printf("Possible d: %Zd\n",d);
	}while(mpz_sgn(d)<0);
	printgmp("p",p);
	printgmp("q",q);
	printgmp("p-1",pneg);
	printgmp("q-1",qneg);
	printgmp("phin",phin);
	printgmp("e",e);
	printgmp("d",d);
	gmp_printf("\nPublic keys: [%Zd,%Zd]\n",e,n);
	mpz_t message;
	mpz_init_set_str(message,argv[3],10);
	cout<<"==========A Sending message============\n";
	mpz_t c;
	mpz_init(c);
	mpz_powm(c,message,e,n);
	gmp_printf("\nCipher text: %Zd\n",c);
	cout<<"=========B Recieveing==================\n";
	mpz_t messagerec;
	mpz_init(messagerec);
	mpz_powm(messagerec,c,d,n);
	gmp_printf("\nMessage text: %Zd\n",messagerec);
	return 0;
}