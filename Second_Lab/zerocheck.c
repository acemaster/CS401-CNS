#include <stdio.h>
#include <gmp.h>
#include <string.h>


int main(int argc, char const *argv[])
{
	mpz_t zero;
	mpz_init_set_str(zero,argv[1],10);
	printf("\n\nResult: %d",mpz_cmp_si(zero,0));
	return 0;
}