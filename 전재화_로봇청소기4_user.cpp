extern void scanFromRobot(int ceilingState[3][3]);
extern int moveRobot(void);
extern void turnRobot(int mCommand);

#include <iostream>
using namespace std;

#define MAX 150
#define INIT_POS 75
#define SCORE_MAX 99999999

int currDir, currY, currX;
int dy[4] = { -1,0,+1,0 };
int dx[4] = { 0,-1,0,+1 };
enum Dir { up, left, down, right }; //4로 나눈 나머지 연산을 통해 방향 전환 후 진행방향을 알 수 있음
enum Cost { scan = 20, turn = 15, move = 10 };

struct Coor
{
    int y, x, dir, score;
};

struct Queue
{
    static int constexpr Queue_Max = 64 * 64 + 10;
    Coor arr[Queue_Max];
    int front, rear;

    Queue() : front(0), rear(0)
    {

    }

    bool empty()
    {
        return front == rear;
    }

    void push(Coor c)
    {
        arr[rear++] = c;
    }

    Coor pop(void)
    {
        return arr[front++];
    }
};

struct Buffer
{
    enum status
    {
        NOT_BLOCK = 0, BLOCK, IN_BUFFER
    };
    
    status map[MAX][MAX];
    int visit[MAX][MAX], visitCheck;
    bool cleaned[MAX][MAX];
    int path[MAX][MAX];
    int min, size;
    Coor min_coor;

    Buffer() : min(SCORE_MAX), size(0)
    {
        for (int i = 0; i < MAX; i++)
        {
            for (int j = 0; j < MAX; j++)
            {
                map[i][j] = BLOCK;
                visit[i][j] = 0;
                cleaned[i][j] = false;
            }
        }
        cleaned[INIT_POS][INIT_POS] = true;
    }

    Coor pop(void)
    {
        map[min_coor.y][min_coor.x] = NOT_BLOCK;
        cleaned[min_coor.y][min_coor.x] = true;
        --size;
        return min_coor;
    }

    void mapping(status floorState, status& map, bool cleaned)
    {
        if (map == status::IN_BUFFER)
            return;
        map = floorState;
        if (map == status::NOT_BLOCK && cleaned == false)
        {
            map = status::IN_BUFFER;
            ++size;
        }
    }

    void mapUpdate(int floorState[3][3])
    {
        int baseY = currY - 1;
        int baseX = currX - 1;
        if (currDir == Dir::up)
        {
            for (int y = 0, sy = 0; y <= 2; y++, sy++)
            {
                for (int x = 0, sx = 0; x <= 2; x++, sx++)
                {
                    mapping((status)floorState[y][x], map[baseY + sy][baseX + sx], cleaned[baseY + sy][baseX + sx]);
                }
            }
        }
        else if (currDir == Dir::left)
        {
            for (int y = 0, sx = 0; y <= 2; y++, sx++)
            {
                for (int x = 0, sy = 2; x <= 2; x++, sy--)
                {
                    mapping((status)floorState[y][x], map[baseY + sy][baseX + sx], cleaned[baseY + sy][baseX + sx]);
                }
            }
        }
        else if (currDir == Dir::down)
        {
            for (int y = 0, sy = 2; y <= 2; y++, sy--)
            {
                for (int x = 0, sx = 2; x <= 2; x++, sx--)
                {
                    mapping((status)floorState[y][x], map[baseY + sy][baseX + sx], cleaned[baseY + sy][baseX + sx]);
                }
            }
        }
        else if (currDir == Dir::right)
        {
            for (int y = 0, sx = 2; y <= 2; y++, sx--)
            {
                for (int x = 0, sy = 0; x <= 2; x++, sy++)
                {
                    mapping((status)floorState[y][x], map[baseY + sy][baseX + sx], cleaned[baseY + sy][baseX + sx]);
                }
            }
        }
    }

    void update(void)
    {
        ++visitCheck;
        min = SCORE_MAX;
        Queue q;
        q.push({ currY, currX, currDir, 0 });
        visit[currY][currX] = visitCheck;

        while (!q.empty())
        {
            Coor now = q.pop();

            if (map[now.y][now.x] == IN_BUFFER)
            {
                if (min > now.score)
                {
                    min = now.score;
                    min_coor = now;
                }
                continue;
            }

            for (int diff = 0; diff < 4; diff++)
            {
                int ndir = (now.dir + diff) % 4;
                int ny = now.y + dy[ndir];
                int nx = now.x + dx[ndir];
                if (map[ny][nx] != BLOCK && visit[ny][nx] != visitCheck)
                {
                    q.push({ ny, nx, ndir, diff == 0 ? now.score + Cost::move : now.score + Cost::turn + Cost::move });
                    visit[ny][nx] = visitCheck;
                    path[ny][nx] = ndir;
                } 
            }
        }
    }

    void FollowPath(Coor now, Coor toGo)
    {
        if (now.y == currY && now.x == currX)
        {
            int diffDir = (path[toGo.y][toGo.x] - path[now.y][now.x] + 4) % 4;
            if (diffDir != 0)
                turnRobot(diffDir);
            moveRobot();
            return;
        }
        int opDir = (path[now.y][now.x] + 2) % 4;
        int ny = now.y + dy[opDir];
        int nx = now.x + dx[opDir];

        FollowPath({ ny, nx }, now);

        int diffDir = (path[toGo.y][toGo.x] - path[now.y][now.x] + 4) % 4;
        if(diffDir != 0)
            turnRobot(diffDir);
        moveRobot();
    }

    void print(void)
    {
        for (int i = 0; i < MAX; i++)
        {
            for (int j = 0; j < MAX; j++)
            {
                cout << map[i][j];
            }
            cout << endl;
        }
        cout << endl;
    }
};

void init(void)
{

}

void cleanHouse(void)
{
    currY = INIT_POS, currX = INIT_POS, currDir = Dir::up;
    int floorState[3][3];
    Buffer buffer;
    Coor dest;
    
    do 
    {
        scanFromRobot(floorState);
        buffer.mapUpdate(floorState);
        buffer.update();

        if (buffer.size == 0)
            break;

        dest = buffer.pop();
        //path 저장된거에 따라 move 및 turn호출
        int opDir = (buffer.path[dest.y][dest.x] + 2) % 4;
        int ny = dest.y + dy[opDir];
        int nx = dest.x + dx[opDir];
        buffer.FollowPath({ny, nx}, dest);
        //currDir, currY, currX 갱신
        currDir = buffer.path[dest.y][dest.x];
        currY = dest.y;
        currX = dest.x;

        //buffer.print(); for debug

    } while (true);
}