/*
Elgammal
================
a -> number 1
b -> number 2
r -> result
================
*/
#include <stdio.h>
#include <gmp.h>
#include <string.h>

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


mpz_t zero;

int main(int argc, char **argv)
{
	printf("==========Elgammal=================\n");
	printf("Please give arguments in the form: ./diffie q m");
	mpz_t q,alpha,m;
	mpz_init_set_str(q,argv[1],10);
	findprimitiveroot(argv,alpha);
	mpz_init_set_str(m,argv[2],10);
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
	mpz_powm(ya,alpha,xa,q);
	gmp_printf("\n\nResult: Private key: Xa: %Zd\n",xa);
	gmp_printf("\n\nResult: Public Key [ya,q,alpha]: [%Zd,%Zd,%Zd]\n",ya,q,alpha);
	printf("\nSecond person sending the message............\n");
	mpz_t c1,c2,Key;
	mpz_init(c1);
	mpz_init(c2);
	mpz_init(Key);
	gmp_randinit_default(state);
	mpz_urandomm(k,state,q);
	gmp_printf("\n\nResult: k: %Zd\n",k);
	mpz_powm(Key,ya,k,q);
	mpz_powm(c1,alpha,k,q);
	mpz_mul(c2,Key,m);
	mpz_fdiv_r(c2,c2,q);
	gmp_printf("\n\nResult: Random k produced: %Zd\n",k);
	gmp_printf("\n\nResult: One time Key K: %Zd\n",Key);
	gmp_printf("\n\nResult: [C1,C2]: [%Zd,%Zd]\n",c1,c2);
	printf("===================================\n");
	printf("A decrpyting message\n");
	mpz_t KeyRec,KeyInv,Message,minus1;
	mpz_init(KeyRec);
	mpz_init(KeyInv);
	mpz_init(Message);
	mpz_init_set_str(minus1,"-1",10);
	mpz_powm(KeyRec,c1,xa,q);
	gmp_printf("\n\nResult: One time Key KeyRec: %Zd\n",KeyRec);
	mpz_powm(KeyInv,Key,minus1,q);
	gmp_printf("\n\nResult: One time Key inverse: %Zd\n",KeyInv);
	mpz_mul(Message,c2,KeyInv);
	mpz_fdiv_r(Message,Message,q);
	gmp_printf("\n\nResult: Message: %Zd\n",Message);
	return 0;
}