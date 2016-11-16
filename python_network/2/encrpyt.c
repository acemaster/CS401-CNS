#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <gmp.h>
#include <stdio.h>


int MAX_KEYS = 2;
struct public_key{
	mpz_t e;
	mpz_t n;
};

struct private_key{
	mpz_t d;
};

struct private_key prt[20];
struct public_key pub[20];

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
	// gmp_printf("\n%Zd and %Zd",a,b);
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
	// gmp_printf("\nInverse Result: \nr: %Zd\nx: %Zd\ny: %Zd\n",r,x,y);
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
		// gmp_printf("Possible prime: %Zd\n",prime);
		mpz_gcd(gcdget,prime,phin);
		if(mpz_cmp(gcdget,one)==0)
			break;
	}
}

void printgmp(char *str,mpz_t p)
{
	printf("%s ",str);
	gmp_printf(": %Zd\n",p);
}

void encrpyt(mpz_t c,mpz_t message,mpz_t e, mpz_t n)
{
	mpz_init(c);
	mpz_powm(c,message,e,n);
	// gmp_printf("\nCipher text: %Zd\n",c);
}

void decrpyt(mpz_t messagerec,mpz_t c,mpz_t d,mpz_t n)
{
	mpz_init(messagerec);
	mpz_powm(messagerec,c,d,n);
}
void generatekey(mpz_t p,mpz_t q,mpz_t e,mpz_t d,mpz_t n)
{
	gmp_randinit_default(state);
	// printf("==========RSA Key gen=================\n");
	mpz_t pneg,qneg;
	mpz_init_set_str(one,"1",10);
	mpz_t phin;
	mpz_init(n);
	mpz_init(phin);
	mpz_init(pneg);
	mpz_init(qneg);
	mpz_mul(n,p,q);
	mpz_sub(pneg,p,one);
	mpz_sub(qneg,q,one);
	mpz_mul(phin,pneg,qneg);
	mpz_t y;
	do
	{
		findprime(phin,e);
		mpz_init(d);
		mpz_init(y);
		findinv(e,phin,d,y);
		// gmp_printf("Possible d: %Zd\n",d);
	}while(mpz_sgn(d)<0);
	// printgmp("p",p);
	// printgmp("q",q);
	// printgmp("p-1",pneg);
	// printgmp("q-1",qneg);
	// printgmp("phin",phin);
	// printgmp("e",e);
	// printgmp("d",d);
	// gmp_printf("\nPublic keys: [%Zd,%Zd]\n",e,n);
}

int main(int argc, char **argv)
{
	//Generate the public and private key list
	if(atoi(argv[1]) == 0) 
	{
		//Generation
		mpz_t e,d,n;
		mpz_t startprime;
		mpz_t p,q;
		mpz_inits(e,d,n,p,q,NULL);
		mpz_init_set_str(startprime,argv[2],10);
		mpz_nextprime(p,startprime);
		int i=0;
		for(i=0;i<MAX_KEYS;i++)
		{
			mpz_nextprime(q,p);
			generatekey(p,q,e,d,n);
			mpz_set(pub[i].e,e);
			mpz_set(pub[i].n,n);
			mpz_set(prt[i].d,d);
			mpz_nextprime(p,q);
		}
		int index = atoi(argv[3]);
		// printf("index: %d",index);
		gmp_printf("%Zd:%Zd:%Zd",pub[index].e,pub[index].n,prt[index].d);
	}
	else if(atoi(argv[1]) == 1)
	{
		//Encrpytion
		mpz_t e,n,d,m;
		mpz_t c;
		mpz_init_set_str(e,argv[2],10);
		mpz_init_set_str(n,argv[3],10);
		mpz_init_set_str(m,argv[4],10);
		mpz_init(c);
		encrpyt(c,m,e,n);
		gmp_printf("%Zd",c);
	}

	else if(atoi(argv[1]) == 2)
	{
		//Decryption
		mpz_t e,n,d,m;
		mpz_t c;
		mpz_init_set_str(d,argv[2],10);
		mpz_init_set_str(n,argv[3],10);
		mpz_init_set_str(m,argv[4],10);
		mpz_init(c);
		decrpyt(c,m,d,n);
		gmp_printf("%Zd",c);
	}
	else if(atoi(argv[1]) == 3)
	{
		mpz_t d,p,key,nextprime;
		mpz_init_set_str(d,argv[2],10);
		mpz_init_set_str(p,argv[3],10);
		mpz_init(key);
		mpz_mod(key,d,p);
		mpz_init(nextprime);
		mpz_nextprime(nextprime,p);
		mpz_mul(key,p,nextprime);
		mpz_add(key,key,d);
		gmp_printf("%Zd:%Zd",key,nextprime);
	}
	return 0;
}