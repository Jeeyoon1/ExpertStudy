// *** mysolution.cpp ***
extern int inquireTo_Oracle(int r1, int c1, int r2, int c2);
extern void report(int r, int c, char v);

#define MAX 128
int buffer[MAX][MAX];
int rowPrefixSum[MAX][MAX];
int reuseVal[MAX];

void makeTreasureMap(int n)
{
	int half = n / 2;

	//2분면
	for (int j = n; j > half; j--)
	{
		buffer[1][j] = inquireTo_Oracle(1, 1, n, j);
		for (int i = 1; i <= half; i++)
		{
			rowPrefixSum[i][j] = buffer[i][j] - (buffer[i + 1][j] = inquireTo_Oracle(i + 1, 1, n, j));
			if (i == half)
				reuseVal[j] = buffer[i + 1][j];
		}
	}
	//4분면
	for (int j = n; j > half; j--)
	{
		buffer[n][j] = buffer[1][j];
		for (int i = n; i > half; i--)
		{
			if (i == half + 1)
				rowPrefixSum[i][j] = buffer[i][j] - (buffer[n][j] - reuseVal[j]);
			else
				rowPrefixSum[i][j] = buffer[i][j] - (buffer[i - 1][j] = inquireTo_Oracle(1, 1, i - 1, j));
		}
	}
	//1분면
	for (int j = 1; j <= half; j++)
	{
		buffer[1][j + 1] = inquireTo_Oracle(1, j + 1, n, n);
		for (int i = 1; i <= half; i++)
		{
			rowPrefixSum[i][j] = rowPrefixSum[i][n] + (buffer[i + 1][j + 1] = inquireTo_Oracle(i + 1, j + 1, n, n)) - buffer[i][j + 1];
			if (i == half)
				reuseVal[j] = buffer[i + 1][j + 1];
		}
	}
	//3분면
	for (int j = 1; j <= half; j++)
	{
		buffer[n][j + 1] = buffer[1][j + 1];
		for (int i = n; i > half; i--)
		{
			if (i == half + 1)
				rowPrefixSum[i][j] = rowPrefixSum[i][n] + (buffer[n][j + 1] - reuseVal[j]) - buffer[i][j + 1];
			else
				rowPrefixSum[i][j] = rowPrefixSum[i][n] + (buffer[i - 1][j + 1] = inquireTo_Oracle(1, j + 1, i - 1, n)) - buffer[i][j + 1];
		}
	}

	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			report(i, j, (rowPrefixSum[i][j] - rowPrefixSum[i][j - 1]) == 1 ? '1' : '0');
		}
	}
}