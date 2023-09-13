#include <cstdio>
typedef unsigned int uint;

uint in, out;

int main(void)
{
	scanf("%u", &in);
	uint bitMask = 0b11111111;

	for (int i = 0; i < 32; i += 8)
	{
		out |= ((in >> i) & bitMask) << (24 - i);
	}

	printf("%u", out);
}