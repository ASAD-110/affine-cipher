#include "EligibleKey.h"

int validA(int a, int m)
{
	/*Checks if a if co-prime to 26*/
	/*m is always 26*/
	int temp = 0;
	while (m != 0)
	{
		temp = m;
		m = a%m;
		a = temp;
	}		
	return a; /*if 1, then co-prime*/
}
