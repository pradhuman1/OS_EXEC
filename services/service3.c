#include <stdio.h>
#include<stdlib.h>
int gcd(int a, int b)
{
	if (b == 0)
		return a;
	return gcd(b, a % b); 
}

int main(int argc, char *argv[])
{
  if(argc!=3){
    printf("Wrong arguments");
    exit(-1);
  }
	int a = atoi(argv[1]), b = atoi(argv[2]);
	printf("GCD of %d and %d is %d \n", a, b, gcd(a, b));
	return 0;
}
