
int main(int argc, char **argv)
{
	unsigned *ptr = (unsigned *) 0x50000000;
	int i;

	for (i = 0; i < 16; i++)
		*ptr = i;

	return 0;
}
