extern void scanFromRobot(int ceilingState[3][3]);
extern int moveRobot(void);
extern void turnRobot(int mCommand);
#include <stdio.h>
#define MAX_UNIT (22)
#define H_UNIT (10)
#define RIGHT(dir) (((dir+3)%4))
int visit[MAX_UNIT][MAX_UNIT];
// 1은 지나갔지만, 다시 돌아오지 못한 경우 되돌아 가기 위한 수단
// 2는 벽취급
short mapp[MAX_UNIT][MAX_UNIT];
int map[64][64];
int move_x[4] = { 0,-1,0,1 };
int move_y[4] = { -1,0,1,0 };
int queue[1000000];
int sr = 0;
int re = 0;
int root_direction;
int floor_state[3][3];
int x, y, dir;
void init(void)
{
    
}
int go_nearest_uncleaned(int x_loc, int y_loc, int direction) {
    sr = 0;
    re = 0;
    int new_x;
    int new_y;
    for (int i = -1; i <= 1; i += 2) {
        if (mapp[x_loc][y_loc + i] != 1){
            if (visit[x_loc][y_loc + i] == 0) {

            }
            queue[sr++] = x_loc;
            queue[sr++] = y_loc + i;
        }
        if (mapp[x_loc + i][y_loc] != 1) {
            queue[sr++] = x_loc + i;
            queue[sr++] = y_loc;
        }
    }
    while (sr > re) {
        new_x = queue[re++];
        new_y = queue[re++];
    }
    return 0;
}
void fill_map(int robot_x, int robot_y, int direction) {

    if (direction == 0) // UP
    {
        for (int y = robot_y - 1, sy = 0; y <= robot_y + 1; y++, sy++)
        {
            for (int x = robot_x - 1, sx = 0; x <= robot_x + 1; x++, sx++)
            {
                mapp[y][x] = floor_state[sy][sx];
                if (floor_state[sy][sx] == 1) {
                    visit[y][x] = 2;
                }
            }
        }
    }
    if (direction == 1) // LEFT
    {
        for (int y = robot_y - 1, sx = 2; y <= robot_y + 1; y++, sx--)
        {
            for (int x = robot_x - 1, sy = 0; x <= robot_x + 1; x++, sy++)
            {
                mapp[y][x] = floor_state[sy][sx];
                if (floor_state[sy][sx] == 1) {
                    visit[y][x] = 2;
                }
            }
        }
    }
    else if (direction == 2) // DOWN
    {
        for (int y = robot_y - 1, sy = 2; y <= robot_y + 1; y++, sy--)
        {
            for (int x = robot_x - 1, sx = 2; x <= robot_x + 1; x++, sx--)
            {
                mapp[y][x] = floor_state[sy][sx];
                if (floor_state[sy][sx] == 1) {
                    visit[y][x] = 2;
                }
            }
        }
    }
    else if (direction == 3) // RIGHT
    {
        for (int y = robot_y - 1, sx = 0; y <= robot_y + 1; y++, sx++)
        {
            for (int x = robot_x - 1, sy = 2; x <= robot_x + 1; x++, sy--)
            {
                mapp[y][x] = floor_state[sy][sx];
                if (floor_state[sy][sx] == 1) {
                    visit[y][x] = 2;
                }
            }
        }
    }
   /* for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            printf("%4d ", mapp[i][j]);
        }
        printf("\n");
    }*/
}
//방문한 곳을 벽처럼 취급하기 위한 함수
void fill_visit() {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            if (visit[i][j] == 1) {
                visit[i][j] = 2;
            }
        }
    }
}
void wall_tracking() {
    int start_x = x;
    int start_y = y;
    int flag = 0;
    while (1) {
        if (flag == 1 && start_x == x && start_y == y) {
            if (visit[y + move_y[dir]][x + move_x[dir]] != 0) {
                fill_visit();
                break;
            }
        }
        //내가 확인 할 곳은 왼쪽, 앞, 앞에서 오른쪽 데각선. 이중 하나라도 밝혀져 있지 않다면 Scan으로 밝히고 간다.
        if (mapp[y + move_y[dir]][x + move_x[dir]] == -1
            || mapp[y + move_y[dir]+ move_y[(dir + 3) % 4]][x + move_x[dir] + move_x[(dir + 3) % 4]] == -1
            || mapp[y + move_y[(dir + 1) % 4]][x+move_x[(dir+1)%4]] == -1)
        {
            scanFromRobot(floor_state);
            fill_map(x, y, dir);
        }
        //앞과 오른쪽길이 열려있으면 우측 벽면을 타기 위한 벽타기
        if (visit[y + move_y[dir]][x + move_x[dir]] != 2 && visit[y + move_y[dir] + move_y[(dir + 3) % 4]][x + move_x[dir] + move_x[(dir + 3) % 4]] != 2) {
            moveRobot();
            turnRobot(3);
            moveRobot();
            visit[y + move_y[dir]][x + move_x[dir]] = 1;
            x += (move_x[dir] + move_x[(dir + 3) % 4]);
            y += (move_y[dir] + move_y[(dir + 3) % 4]);
            visit[y][x] = 1;
            dir = (dir + 3) % 4;
            flag = 1;
        } 
        //앞은 열려있고 우측 길은 열려있지 않다면 직진
        else if (visit[y + move_y[dir]][x + move_x[dir]] != 2) {
            moveRobot();
            x += move_x[dir];
            y += move_y[dir];
            visit[y][x] = 1;
            flag = 1;
        }
        // 앞이 막혀있다면 왼쪽을 확인해보고, 왼쪽이 막혀있다면 한바퀴 회전, 막혀있지 않다면 왼쪽으로 회전
        else if (visit[y + move_y[dir]][x + move_x[dir]] == 2) {
            if (visit[y + move_y[(dir + 1) % 4]][x + move_x[(dir + 1) % 4]] == 2) {
                turnRobot(2);
                dir = (dir + 2) % 4;
            }
            else {
                turnRobot(1);
                dir = (dir + 1) % 4;
            }
        }
        
    }
}
void cleanHouse(void)
{
   //first try
    for (int i = 0; i < MAX_UNIT; i++) {
        for (int j = 0; j < MAX_UNIT; j++) {
            visit[i][j] = 0;
            mapp[i][j] = -1;
        }
    }
    x = H_UNIT;
    y = H_UNIT;
    dir = 0;
    while (1) {
        scanFromRobot(floor_state);
        fill_map(x, y, dir);
        
        if (floor_state[0][1] == 0) {
            moveRobot();
        }
        if (floor_state[0][1] == 1) {
            turnRobot(1);
            dir = 1;
            wall_tracking();
            break;
        }
    }
    int sum = 0;
    for (int i = 0; i < MAX_UNIT; i++) {
        for (int j = 0; j < MAX_UNIT; j++) {
            sum += visit[i][j];
        }
    }
    if (sum == 200) {
        return;
    }
}
