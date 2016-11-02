/*
Diffie Hillman
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

void findprimitiveroot(char **argv,mpz_t proot)
{
	printf("==========Primitive Root=================\n");
	mpz_t q,qneg,one,two,qpow;
	mpz_init(qneg);
	mpz_init(qpow);
	mpz_init_set_str(one,"1",10);
	mpz_init_set_str(two,"2",10);
	mpz_init_set_str(q,argv[1],10);
	mpz_sub(qneg,q,one);
	mpz_fdiv_q(qpow,qneg,two);
	gmp_printf("\n\nResult: q: %Zd\n",q);
	mpz_t pproot;
	mpz_init(proot);
	mpz_init(pproot);
	int k=0;
	gmp_randstate_t state;
	gmp_randinit_default(state);
	while(1)
	{
		mpz_urandomm(proot,state,q);
		gmp_printf("\n\nResult: possible alpha: %Zd\n",proot);
		// mpz_init_set_str(proot,"10",10);
		mpz_powm(pproot,proot,qpow,q);
		// gmp_printf("\n\nResult: alpha: %Zd\n",proot);
		// gmp_printf("\n\nResult: qneg: %Zd\n",qneg);
		k++;
		if(mpz_cmp(pproot,qneg) == 0 or k>5)
			break;
	}
	gmp_printf("\n\nResult: alpha: %Zd\n",proot);
}


int main(int argc, char **argv)
{
	printf("==========Diffie=================\n");
	printf("Please give arguments in the form: ./diffie q ");
	mpz_t q,alpha;
	mpz_init_set_str(q,argv[1],10);
	findprimitiveroot(argv,alpha);
	gmp_printf("\n\nResult: q: %Zd\nalpha: %Zd\n",q,alpha);
	mpz_t xa,ya,xb,yb,k,kr;
	mpz_init(xa);
	mpz_init(xb);
	mpz_init(ya);
	mpz_init(yb);
	mpz_init(k);
	mpz_init(kr);
	gmp_randstate_t state;
	gmp_randinit_default(state);
	mpz_urandomm(xa,state,q);
	gmp_printf("\n\nResult: Xa: %Zd\n",xa);
	mpz_urandomm(xb,state,q);
	gmp_printf("\n\nResult: Xb: %Zd\n",xb);
	mpz_powm(ya,alpha,xa,q);
	mpz_powm(yb,alpha,xb,q);
	gmp_printf("\n\nResult: Public Key ya: %Zd\n",ya);
	gmp_printf("\n\nResult: Public Key yb: %Zd\n",yb);
	mpz_powm(k,yb,xa,q);
	gmp_printf("\n\nResult: Key Generated k: %Zd\n",k);
	mpz_powm(kr,ya,xb,q);
	gmp_printf("\n\nResult: Key Recived k: %Zd\n",kr);
	if(mpz_cmp(k,kr)==0)
	{
		printf("\nBoth the keys are same:Success\n");
	}
	return 0;
}