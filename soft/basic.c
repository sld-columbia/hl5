#include <stdlib.h>

int ag = 200, bg = 300, cg = 400;

int main()
{
	int a = 5, b = 10, c = 100;

	c = a + b;

	b = a + c;

	a = b + c;

	ag = a;
	
	bg = b;

	cg = c;

	return 0;
}
