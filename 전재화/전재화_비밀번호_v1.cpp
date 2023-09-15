#include <cstdio>
typedef unsigned long long ull;

enum {SMALL, LARGE};

int main(void)
{
	ull in, small, large;
	int count = 0, onesIndex[2][70] = { 0, };
	scanf("%llu", &in);
	
	ull bitMask;
	for (int idx = 0; (bitMask = 1ULL << idx) <= in; idx++)
		if (in & bitMask)
		{
			onesIndex[SMALL][count] = idx;
			onesIndex[LARGE][count++] = idx;
		}
		
	small = 0;
	for (int i = 0; i < count; i++)
	{
		if (onesIndex[SMALL][i] > i)
		{
			// 0다음에 나오는 1을 한칸 LSB쪽으로 옮기기
			--onesIndex[SMALL][i];
			// 옮긴 칸보다 LSB쪽에 위치한 1들을 MSB쪽으로 몰기
			for (int j = i - 1; j >= 0; j--)
				onesIndex[SMALL][j] = onesIndex[SMALL][j + 1] - 1;
			// small 설정하기
			for (int j = 0; j < count; j++)
				small |= 1ULL << onesIndex[SMALL][j];
			break;
		}
	}

	large = 0;
	for (int i = 0; i < count; i++)
	{
		ull bitMask = 1ULL << (onesIndex[LARGE][i] + 1);
		if (!(in & bitMask))
		{
			// 0이전에 나오는 1을 한칸  MSB옮기기
			++onesIndex[LARGE][i];
			// 옮긴 칸보다 LSB쪽에 위치한 1들을 LSB쪽으로 몰기
			for (int j = 0; j < i; j++)
				onesIndex[LARGE][j] = j == 0 ? 0 : onesIndex[LARGE][j - 1] + 1;
			// large 설정하기
			for (int j = 0; j < count; j++)
				large |= 1ULL << onesIndex[LARGE][j];
			break;
		}
	}
	printf("%llu %llu", small, large);
}