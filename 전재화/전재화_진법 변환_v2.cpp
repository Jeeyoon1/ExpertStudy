#include <cstdio>

typedef long long ll;
int A, B;
const char* lookUptable = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char S[100];

void getAns(ll num, int B)
{
	if (num < B)
	{
		printf("%c", lookUptable[num]);
		return;
	}
	getAns(num / B, B);
	printf("%c", lookUptable[num % B]);
}

int main(void)
{
	ll decimalNum;
	while (scanf("%d", &A) && A)
	{
		scanf("%s %d", S, &B);
		decimalNum = 0;
		for (int i = 0; S[i]; i++)
		{
			if (S[i] < 'A')
				decimalNum = decimalNum * A + S[i] - '0';
			else
				decimalNum = decimalNum * A + S[i] - 'A' + 10;
		}
		getAns(decimalNum, B);
		puts("");
	}
}