extern void scanFromRobot(int ceilingState[3][3]);
extern int moveRobot(void);
extern void turnRobot(int mCommand);

#define MAX 150
#define INIT_POS 75

int currDir, subTask;
int visit[MAX][MAX];
int dy[4] = { -1,0,+1,0 };
int dx[4] = { 0,-1,0,+1 };
enum Dir { up, left, down, right }; //4로 나눈 나머지 연산을 통해 방향 전환 후 진행방향을 알 수 있음
#define WALL 1

void init(void)
{

}

void dfs(int y, int x, int initDir)
{
    visit[y][x] = subTask;

    int floorState[3][3];
    scanFromRobot(floorState);

    for (int i = 0; i < 4; i++)
    {
        if (floorState[1 + dy[i]][1 + dx[i]] == WALL)
            continue;
        //initDir 기준으로 절대Dir 구하기
        int nextDir = (initDir + i) & 3;
        int ny = y + dy[nextDir];
        int nx = x + dx[nextDir];
        //이미 방문했으면 다음 방향 탐색
        if (visit[ny][nx] == subTask)
            continue;
        //dfs를 하고 오면 로봇의 방향이 달라질 수 있음 => currDir기준으로 돌려야할 방향 선정
        turnRobot((nextDir - currDir + 4) & 3);
        moveRobot();
        currDir = nextDir;
        dfs(ny, nx, nextDir);
    }
    //initDir과 반대방향으로 진행해야 그 전 구역으로 돌아갈 수 있다
    int nextDir = (initDir + 2) & 3;
    turnRobot((nextDir - currDir + 4) & 3);
    moveRobot();
    currDir = nextDir;
}

void cleanHouse(void)
{
    currDir = up;
    ++subTask;
    //초기방향을 절대Dir의 up으로 설정
    dfs(INIT_POS, INIT_POS, up);
}