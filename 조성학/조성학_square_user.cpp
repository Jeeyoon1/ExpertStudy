#include <cstdio>
#include <cstring>

const int LM = 66;
const int MINLEN = 10;

int A[LM][LM];
int shiftimg[LM][LM];

void printA()
{
    for (int i = 1; i < LM; i++)
    {
        for (int j = 1; j < LM; j++)
        {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("\n");
}

void printImage()
{
    for (int i = 1; i < LM; i++)
    {
        for (int j = 1; j < LM; j++)
        {
            printf("%d ", shiftimg[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("\n");
}

int checkSquare()
{
    int ans = 0;
    
    for (int i = 1; i < LM-MINLEN; i++)
    {
        for (int j = 1; j < LM - MINLEN; j++)
        {
            if(A[i][j]>0)
            {
                for (int er = i + MINLEN; er < LM; er++) 
                {
                    for (int ec = j + MINLEN; ec < LM; ec++) 
                    {
                        if (A[er][ec] > 0) {
                            if (A[i][j] > 0)
                            {
                                if (A[i][ec] < 0 && A[er][j] < 0)
                                {
                                    A[i][j]--, A[er][ec]--;
                                    A[i][ec]++, A[er][j]++;

                                    if (er - i == ec - j)
                                        ans++;
                                }
                            }
                        }
                    }
                 }
            }
        }
    }
    
    //printA();
    return ans;
}

int recog(unsigned char image[64][64])
{
    int ans = 0;

    for (int i = 0; i < 64; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            shiftimg[i + 1][j + 1] = image[i][j];
        }
    }
    /*
    for (int i = 1; i < LM; i++)
    {
        for (int j = 1; j < LM; j++)
        {
            if (shiftimg[i][j] > 0)
            {
                for (int ec = j + MINLEN; ec < LM; ec++)
                {
                    if (shiftimg[i][ec - 1] > shiftimg[i][ec])
                    {
                        for (int er = i + MINLEN; er < LM; er++)
                        {
                            if ((shiftimg[er - 1][j] > shiftimg[er][j]) && (shiftimg[er - 1][ec] > shiftimg[er][ec]))
                            {
                                for (int a = i; a <= er; a++)
                                {
                                    for (int b = j; b <= ec; b++)
                                    {
                                        shiftimg[a][b]--;
                                    }
                                }

                                if (ec - j == er - i)
                                    ans++;
                            }
                        }
                    }
                }
            }
        }
    }
    */
    
    for (int i = 1; i < LM; i++)
    {
        for (int j = 1; j < LM; j++)
        {
            A[i][j] = shiftimg[i][j] - shiftimg[i][j - 1] - shiftimg[i - 1][j] + shiftimg[i - 1][j - 1];
        }
    }
    
    /*
    memset(shiftimg, 0, sizeof(shiftimg));
    memset(A, 0, sizeof(A));
    for (int i = 1; i <= 10; i++)
    {
        for (int j = 1; j <= 10; j++)
        {
            shiftimg[i][j] = 1;
        }
    }
    for (int i = 11; i <= 15; i++)
    {
        for (int j = 1; j <= 5; j++)
        {
            shiftimg[i][j] = 1;
        }
    }
    for (int i = 1; i < LM; i++)
    {
        for (int j = 1; j < LM; j++)
        {
            A[i][j] = shiftimg[i][j] - shiftimg[i][j - 1] - shiftimg[i - 1][j] + shiftimg[i - 1][j - 1];
        }
    }
    */
//    printA();
//    printImage();
    ans = checkSquare();

    return ans;
}
