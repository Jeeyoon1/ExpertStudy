#include <cstdio>

int A, B;
char S[100];
typedef long long int ll;

int char2int(char c)
{
	if (c >= 'A' && c <= 'Z')
		return c - 'A' + 10;
	return c - '0';
}

char int2char(int n)
{
	if (n >= 10 && n <= 10 + ('Z' - 'A'))
		return 'A' + (n - 10);
	return '0' + n;
}

ll HornerMethodEngine(int weight, char* num)
{
	ll ret = 0;
	int idx = 0;
	while (num[idx] != '\0')
	{
		ret = ret * weight + char2int(num[idx]);
		++idx;
	}
	return ret;
}

void getAns(int weight, ll num, char* ans, int& size)
{
	if (num == 0) //0에 대한 예외처리
	{
		ans[0] = '0';
		size = 1;
		return;
	}

	ll rem = num;
	while (rem > 0)
	{
		ans[size++] = int2char(rem % weight);
		rem /= weight;
	}
}

int main(void)
{
	while (true)
	{
		scanf("%d", &A);
		if (A == 0)
			break;

		int i = 0;
		char c = getchar(); //공백 제거
		while (i < sizeof(S) - 1 && (c = getchar()) != ' ')
			S[i++] = c;
		S[i] = '\0';

		scanf("%d", &B);

		ll decimalNum = HornerMethodEngine(A, S);

		char ans[100];
		int size = 0;
		getAns(B, decimalNum, ans, size);

		for (int i = size - 1; i >= 0; i--)
			printf("%c", ans[i]);
		printf("\n\n\n");
	}
}