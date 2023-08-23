#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <cstring>
#include <random>

extern void init(void);
extern void cleanHouse(void);

#define MAX_N (64)
#define MAX_SUB_TASK (10)

static int mLimitMoveCnt = 0;
static int houseInfo[MAX_N][MAX_N];
static int isCleaned[MAX_N][MAX_N];
static int robotInfo[3] = { 0, }; // [0] : y, [1] : x, [2] : direction
static int score = 0;

static int dy[4] = { -1, 0, 1, 0 };
static int dx[4] = { 0, -1, 0, 1 };

static bool ok = false;

void scanFromRobot(int floorState[3][3])
{
    int robot_y = robotInfo[0];
    int robot_x = robotInfo[1];
    int direction = robotInfo[2];

    if (direction == 0) // UP
    {
        for (int y = robot_y - 1, sy = 0; y <= robot_y + 1; y++, sy++)
        {
            for (int x = robot_x - 1, sx = 0; x <= robot_x + 1; x++, sx++)
            {
                floorState[sy][sx] = houseInfo[y][x];
            }
        }
    }
    if (direction == 1) // LEFT
    {
        for (int y = robot_y - 1, sx = 2; y <= robot_y + 1; y++, sx--)
        {
            for (int x = robot_x - 1, sy = 0; x <= robot_x + 1; x++, sy++)
            {
                floorState[sy][sx] = houseInfo[y][x];
            }
        }
    }
    else if (direction == 2) // DOWN
    {
        for (int y = robot_y - 1, sy = 2; y <= robot_y + 1; y++, sy--)
        {
            for (int x = robot_x - 1, sx = 2; x <= robot_x + 1; x++, sx--)
            {
                floorState[sy][sx] = houseInfo[y][x];
            }
        }
    }
    else if (direction == 3) // RIGHT
    {
        for (int y = robot_y - 1, sx = 0; y <= robot_y + 1; y++, sx++)
        {
            for (int x = robot_x - 1, sy = 2; x <= robot_x + 1; x++, sy--)
            {
                floorState[sy][sx] = houseInfo[y][x];
            }
        }
    }

    score += 20;
}

void turnRobot(int mCommand)
{
    score += 15;

    if (mCommand <= 0 || mCommand >= 4)
        return;

    int next_dir = (robotInfo[2] + mCommand) % 4;
    robotInfo[2] = next_dir;
}

int moveRobot(void)
{
    score += 10;
    
    int now_dir = robotInfo[2];
    int sy = robotInfo[0] + dy[now_dir];
    int sx = robotInfo[1] + dx[now_dir];

    if (houseInfo[sy][sx] == 1)
    {
        return 0;
    }
    else
    {
        robotInfo[0] = sy;
        robotInfo[1] = sx;
        isCleaned[sy][sx] = 1;
    }

    return 1;
}

int getRandomNumber() 
{
    std::random_device rd;  // 시드 값을 생성하는 랜덤 장치
    std::mt19937 gen(rd()); // Mersenne Twister 알고리즘을 사용하는 생성기
    std::uniform_int_distribution<> dis; // 기본적으로 int의 범위 내에서 균등 분포 생성

    return dis(gen); // 랜덤값 반환
}

static bool run()
{
    init();

    for (int y = 0; y < MAX_N; y++)
    {
        for (int x = 0; x < MAX_N; x++)
        {
            scanf("%d", &houseInfo[y][x]);
            isCleaned[y][x] = 0;
        }
    }

    for (int i = 0; i < MAX_SUB_TASK; i++)
    {
        memset(isCleaned, 0, sizeof(isCleaned));

        do 
        {
			robotInfo[0] = getRandomNumber() % (MAX_N - 2);
			robotInfo[1] = getRandomNumber() % (MAX_N - 2);
			robotInfo[2] = getRandomNumber() % 4;
		} while (houseInfo[robotInfo[0]][robotInfo[1]] == 1);

        isCleaned[robotInfo[0]][robotInfo[1]] = 1;

        cleanHouse();

        for (int y = 0; y < MAX_N; y++)
        {
            for (int x = 0; x < MAX_N; x++)
            {
                if (houseInfo[y][x] == 1)
                    continue;

                if (isCleaned[y][x] == 0)
                    return false;
            }
        }
    }

    return true;
}

int main()
{
    setbuf(stdout, NULL);
    FILE* file = freopen("robotCleaner4_input.txt", "r", stdin);
    if (!file) {
        printf("Error opening the file.\n");
        return 1;
    }
    int T = 10;

    for (int tc = 1; tc <= T; tc++)
    {
        if (run() == false)
        {
            printf("Fail\n");
            exit(0);
        }
    }
    printf("Score : %d\n", score);
    return 0;
}
