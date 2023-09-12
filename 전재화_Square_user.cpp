/// ***** user.cpp *****
#define MAX 70
#define SIZE 64
#define MIN_LEN 10
int prefixSum[MAX][MAX];
int num[MAX][MAX];

void countSquare(int sy, int sx, int& ret)
{
    for (int yLen = MIN_LEN; yLen < SIZE && sy + yLen <= SIZE + 1 && num[sy][sx] > 0; yLen++)
        for (int xLen = MIN_LEN; xLen < SIZE && sx + xLen <= SIZE + 1 && num[sy][sx] > 0; xLen++)
            if (num[sy + yLen][sx] < 0 && num[sy][sx + xLen] < 0 && num[sy + yLen][sx + xLen] > 0)
            {
                --num[sy][sx];
                ++num[sy + yLen][sx];
                ++num[sy][sx + xLen];
                --num[sy + yLen][sx + xLen];
                if (xLen == yLen)
                    ++ret;
            }
}

int recog(unsigned char image[64][64])
{
    int ret = 0;

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            prefixSum[i + 1][j + 1] = image[i][j];

    for (int i = 1; i < MAX; i++)
        for (int j = 1; j < MAX; j++)
            num[i][j] = prefixSum[i][j] - prefixSum[i][j - 1] - prefixSum[i - 1][j] + prefixSum[i - 1][j - 1];

    for (int i = 1; i <= SIZE + 1 - MIN_LEN; i++)
        for (int j = 1; j <= SIZE + 1 - MIN_LEN; j++)
            if (num[i][j] > 0)
                countSquare(i, j, ret);

    return ret;
}
