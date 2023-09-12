// *** manager.cpp ***
#ifndef _CRT_SECURE_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS 
#endif 

#include<stdio.h>
#include<stdlib.h>
#define MAXN 110

extern void makeTreasureMap(int);
static int N;
static char island[MAXN][MAXN];
static int S[MAXN][MAXN];
static long long usedEnergy;

int inquireTo_Oracle(int r1, int c1, int r2, int c2) {
    if (!(1 <= r1 && r1 <= r2 && r2 <= N && 1 <= c1 && c1 <= c2 && c2 <= N)) {
        printf("Invalid 'inquireTo_Oracle' call");
        exit(0);
    }
    int Ans = S[r2][c2] - S[r1 - 1][c2] - S[r2][c1 - 1] + S[r1 - 1][c1 - 1];
    usedEnergy += (long long)(1 + N * N - (r2 - r1 + 1) * (c2 - c1 + 1));
    return Ans;
}

static bool visit[MAXN][MAXN];
static char result[MAXN][MAXN];
static int result_count;


void report(int r, int c, char v) {
    if (!(1 <= r && r <= N && 1 <= c && c <= N && '0' <= v && v <= '1')) {
        printf("Invalid call\n");
        exit(0);
    }
    if (visit[r][c]) {
        printf("Already called range\n");
        exit(0);
    }
    visit[r][c] = 1;
    result[r][c] = v;
    result_count++;
}

int main() {
    scanf("%d", &N);
    for (int i = 1; i <= N; i++) {
        scanf("%s", &island[i][1]);
    }
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            S[i][j] = S[i - 1][j] + S[i][j - 1] - S[i - 1][j - 1];
            if (island[i][j] == '1') S[i][j] += 1;
        }
    }

    makeTreasureMap(N);
    if (result_count != N * N) {
        printf("Didn't call 'report' for all points");
        exit(0);
    }
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (island[i][j] != result[i][j]) {
                printf("Wrong answer");
                exit(0);
            }
        }
    }
    printf("Answer is Correctly\n");
    printf("Points : %lld\n", usedEnergy);
    return 0;
}
