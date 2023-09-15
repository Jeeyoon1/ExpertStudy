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
			// 0������ ������ 1�� ��ĭ LSB������ �ű��
			--onesIndex[SMALL][i];
			// �ű� ĭ���� LSB�ʿ� ��ġ�� 1���� MSB������ ����
			for (int j = i - 1; j >= 0; j--)
				onesIndex[SMALL][j] = onesIndex[SMALL][j + 1] - 1;
			// small �����ϱ�
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
			// 0������ ������ 1�� ��ĭ  MSB�ű��
			++onesIndex[LARGE][i];
			// �ű� ĭ���� LSB�ʿ� ��ġ�� 1���� LSB������ ����
			for (int j = 0; j < i; j++)
				onesIndex[LARGE][j] = j == 0 ? 0 : onesIndex[LARGE][j - 1] + 1;
			// large �����ϱ�
			for (int j = 0; j < count; j++)
				large |= 1ULL << onesIndex[LARGE][j];
			break;
		}
	}
	printf("%llu %llu", small, large);
}