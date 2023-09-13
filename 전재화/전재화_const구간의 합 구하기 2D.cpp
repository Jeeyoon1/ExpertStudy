#include <cstdio>

int N, M;
long long int acc[1001][1001];

int main(void)
{
	int sr, sc, er, ec;
	int input;
	scanf("%d", &N);
	for (int i = 1; i <= N; i++)
		for (int j = 1; j <= N; j++)
		{
			scanf("%d", &input);
			acc[i][j] = acc[i][j - 1] + acc[i - 1][j] - acc[i-1][j-1]  + input;
		}

	scanf("%d", &M);
	for (int i = 0; i < M; i++)
	{
		scanf("%d %d %d %d", &sr, &sc, &er, &ec);
		printf("%lld\n", acc[er][ec] - acc[sr - 1][ec] - acc[er][sc - 1] + acc[sr - 1][sc - 1]);
	}
}