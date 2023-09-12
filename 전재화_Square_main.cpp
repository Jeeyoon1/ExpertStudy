/// ***** main.cpp *****
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <cstdio> 
const int SIZE = 64;
int COUNT, SCORE;
int seed = 1117;   // will be changed -> 99 fail

unsigned char image[SIZE][SIZE];

extern int recog(unsigned char image[SIZE][SIZE]);
static int pseudo_rand(void) {
    seed = seed * 214013 + 2531011;
    return (seed >> 16) & 0x7FFF;
}

void rect() {
    int sx, sy, ex, ey, tmp;
    while (true) {
        sx = pseudo_rand() % SIZE;
        ex = pseudo_rand() % SIZE;
        sy = pseudo_rand() % SIZE;
        ey = pseudo_rand() % SIZE;

        if (sx > ex) {
            tmp = sx;
            sx = ex;
            ex = tmp;
        }

        if (sy > ey) {
            tmp = sy;
            sy = ey;
            ey = tmp;
        }
        if (ex - sx < 9) continue;
        if (ey - sy < 9) continue;
        break;
    }
    for (register int y = sy; y <= ey; y++)
        for (register int x = sx; x <= ex; x++)
            image[y][x]++;
    if (ex - sx == ey - sy) {
        COUNT++;
    }

}

void _build() {
    for (int y = 0; y < SIZE; y++)
        for (int x = 0; x < SIZE; x++)
            image[y][x] = 0;
    COUNT = 0;
    for (int i = 0; i < 50; i++) rect();
}

int main() {
    SCORE = 0;
    int TC = 10000;
    int up = 0, down = 0;
    for (int T = 0; T < TC; T++)
    {
        _build();
        //output();
        int userans = recog(image);
        if (userans == COUNT) SCORE++;
        //printf("%d %d\n", userans, COUNT);
        else if (userans > COUNT) ++up;
        else ++down;
    }
    printf("up : %d / down : %d\n", up, down);
    if (SCORE >= 8200)
        printf("PASS\n");
    else
        printf("FAIL\n");
    printf("RESULT: %d\n", SCORE);
    return 0;
}