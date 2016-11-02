/*
Primitive Root
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

int main(int argc, char **argv)
{
	printf("==========Primitive Root=================\n");
	printf("Please give arguments in the form: ./pr q ");
	mpz_t q,qneg,one,two,qpow;
	mpz_init(qneg);
	mpz_init(qpow);
	mpz_init_set_str(one,"1",10);
	mpz_init_set_str(two,"2",10);
	mpz_init_set_str(q,argv[1],10);
	mpz_sub(qneg,q,one);
	mpz_fdiv_q(qpow,qneg,two);
	gmp_printf("\n\nResult: q: %Zd\n",q);
	mpz_t pproot,proot;
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
	return 0;
}