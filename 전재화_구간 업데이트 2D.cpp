#include <cstdio>
//hint => prefix sum을 이용해 사각형을 "만들 수" 있음
//prefixSum[i][j] => (1,1)~(i,j)까지의 합

#define MAX 1002 //++er, ++ec일 때, 1001 index가 들어올 수 있음
int N, Q;
int num[MAX][MAX];
long long int prefixSum[MAX][MAX]; 

int main(void)
{
	scanf("%d %d", &N, &Q);
	int sr, sc, er, ec, val;
	for (int i = 0; i < Q; i++)
	{
		scanf("%d %d %d %d %d", &sr, &sc, &er, &ec, &val);
		++er, ++ec;
		num[sr][sc] += val, num[sr][ec] -= val;
		num[er][sc] -= val, num[er][ec] += val;
	}

	for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j <= N; j++)
		{
			prefixSum[i][j] = prefixSum[i - 1][j] + prefixSum[i][j - 1] - prefixSum[i - 1][j - 1] + num[i][j];
			printf("%lld ", prefixSum[i][j]);
		}
		printf("\n");
	}
}