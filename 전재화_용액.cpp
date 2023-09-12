#include <iostream>

int N;
int characterValue[100000];

int inline returnABS(int n)
{
	return n > 0 ? n : -n;
}

int main(void)
{
	std::cin >> N;
	for (int i = 0; i < N; i++)
		std::cin >> characterValue[i];

	int left = 0, right = N - 1;
	int ansLeft, ansRight;
	int sum;
	long long int ans = 3000000000, absVal;
	while (left != right)
	{
		sum = characterValue[left] + characterValue[right];
		if (ans > (absVal = returnABS(sum)))
		{
			ans = absVal;
			ansLeft = characterValue[left];
			ansRight = characterValue[right];
		}
		if (sum == 0)
			break;
		else if (sum > 0)
			--right;
		else
			++left;
	}
	std::cout << ansLeft << " " << ansRight << std::endl;
}