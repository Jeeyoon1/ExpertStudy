#include <cstdio>

int N,M;
int num[1000000];
long long int acc[1000001];

int main(void)
{
	scanf("%d", &N);
	int input;
	for (int i = 1; i <= N; i++)
	{
		scanf("%d", &input);
		acc[i] = acc[i - 1] + input;
	}

	int s, e;
	scanf("%d", &M);
	for (int i = 0; i < M; i++)
	{
		scanf("%d %d", &s, &e);
		printf("%lld\n", acc[e] - acc[s - 1]);
	}
}