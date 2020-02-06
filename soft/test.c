#include <stdio.h>

int main(int argc, char **argv)
{
	unsigned char input[4] = {1, 2, 3, 4};
	unsigned char output[4] = {0xff, 0xff, 0xff, 0xff};
	int i;

	for (int i = 0; i < 4; i++)
		output[i] = input[i] * 3;

	return 0;
}
