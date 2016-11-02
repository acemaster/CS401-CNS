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


int MAX_KEYS = 3;
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
	gmp_printf("\nCipher text: %Zd\n",c);
}

void decrpyt(mpz_t messagerec,mpz_t c,mpz_t d,mpz_t n)
{
	mpz_init(messagerec);
	mpz_powm(messagerec,c,d,n);
}

void generatekey(mpz_t p,mpz_t q,mpz_t e,mpz_t d,mpz_t n)
{
	gmp_randinit_default(state);
	printf("==========RSA Key gen=================\n");
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
	printgmp("p",p);
	printgmp("q",q);
	printgmp("p-1",pneg);
	printgmp("q-1",qneg);
	printgmp("phin",phin);
	printgmp("e",e);
	printgmp("d",d);
	// gmp_printf("\nPublic keys: [%Zd,%Zd]\n",e,n);
}

void printpubpr()
{
	int i=0;
	for(i=0;i<MAX_KEYS;i++)
	{
		gmp_printf("\nPublic keys: [%Zd,%Zd]\n",pub[i].e,pub[i].n);
		gmp_printf("Private keys: [%Zd]\n",prt[i].d);
	}
}


int main(int argc, char **argv)
{
	//Generate the public and private key list
	mpz_t e,d,n;
	mpz_t startprime;
	mpz_t p,q;
	mpz_inits(e,d,n,p,q,NULL);
	mpz_init_set_str(startprime,argv[1],10);
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
	printpubpr();
	//Table generation complete
	int listenfd=0,connfd=0;
	struct sockaddr_in serv_addr;
	char sendbuff[1025];
	char recvbuff[1025];
	time_t t;
	listenfd = socket(AF_INET,SOCK_STREAM,0);
	memset(&serv_addr,'0',sizeof(serv_addr));
	memset(sendbuff,'0',sizeof(sendbuff));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[2]));
	bind(listenfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
	listen(listenfd,10);
	while(1)
	{
		connfd = accept(listenfd,(struct sockaddr*)NULL,NULL);
		printf("client recived\n");
		int pid = fork();
		if (pid == 0)
		{
			mpz_t messagerec,messagerec_dec;
			memset(recvbuff,'0',sizeof(recvbuff));
			while(read(connfd,recvbuff,sizeof(recvbuff))>0)
			{
				printf("%s recieved\n",recvbuff);
				char publicid[10];
				char message[10];
				int i=0;
				for(i=0;recvbuff[i]!='\0';i++)
				{
					if(recvbuff[i] == '|')
					{
						publicid[i]='\0';
						break;
					}
					publicid[i]=recvbuff[i];
				}
				int k=0;
				i=i+1;
				for(;recvbuff[i]!='\0';i++)
				{
					if(recvbuff[i] == '|')
					{
						message[k++]='\0';
						break;
					}
					message[k++]=recvbuff[i];
				}
				printf("message : %s",message);
				printf("Public id: %s \nMessage: %s",publicid,message);
				mpz_init_set_str(messagerec,message,10);
				gmp_printf("\nDecrpyting using %Zd and %Zd",prt[atoi(publicid)].d,pub[atoi(publicid)].n);
				decrpyt(messagerec_dec,messagerec,prt[atoi(publicid)].d,pub[atoi(publicid)].n);
				gmp_printf("\nMessage recieved: %Zd\n",messagerec_dec);
			}
		}
		else
		{
			close(connfd);
		}
	}
	return 0;
}