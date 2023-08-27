extern void scan(int floorState[3][3]);
extern int move(void);
extern void turn(int mCommand);

#define MAX 150
#define INIT_POS 75
#define SCORE_MAX 99999999

int currDir, currY, currX;
int dy[4] = { -1,0,+1,0 };
int dx[4] = { 0,-1,0,+1 };
enum Dir { UP, LEFT, DOWN, RIGHT }; //4로 나눈 나머지 연산을 통해 방향 전환 후 진행방향을 알 수 있음
enum Cost { SCAN = 20, TURN = 15, MOVE = 10 };

struct Coor
{
    int y, x, dir, score;
};

struct PriorityQueue
{
    static int constexpr QueueMax = 64 * 64 + 10;
    Coor arr[QueueMax];
    int size;

    PriorityQueue() : size(0)
    {

    }

    void swap(Coor& a, Coor& b)
    {
        Coor temp = a;
        a = b;
        b = temp;
    }

    bool empty(void)
    {
        return size == 0;
    }

    void push(Coor val)
    {
        arr[size++] = val;

        int currIdx = size - 1;
        int parentIdx;
        while (currIdx > 0)
        {
            parentIdx = (currIdx - 1) / 2;
            if (arr[currIdx].score < arr[parentIdx].score)
            {
                swap(arr[currIdx], arr[parentIdx]);
                currIdx = parentIdx;
            }
            else
                break;
        }
    }

    Coor pop(void)
    {
        Coor popVal = arr[0];

        arr[0] = arr[size - 1];
        --size;

        int currIdx = 0;
        int left, right, smallestIdx;
        while (true)
        {
            left = currIdx * 2 + 1;
            right = currIdx * 2 + 2;
            smallestIdx = currIdx;

            if (left < size && arr[left].score < arr[smallestIdx].score)
                smallestIdx = left;

            if (right < size && arr[right].score < arr[smallestIdx].score)
                smallestIdx = right;

            if (smallestIdx != currIdx)
            {
                swap(arr[currIdx], arr[smallestIdx]);
                currIdx = smallestIdx;
            }
            else
                break;
        }

        return popVal;
    }
};

struct Robot
{
    enum status
    {
        NOT_BLOCK = 0, BLOCK, NEED_CLEANING, NEED_SCAN
    };
    
    int map[MAX][MAX];
    int visit[MAX][MAX], visitCheck;
    bool cleaned[MAX][MAX];
    int path[MAX][MAX];
    int min, size;
    Coor min_coor;

    Robot() : min(SCORE_MAX), size(0)
    {
        for (int i = 0; i < MAX; i++)
        {
            for (int j = 0; j < MAX; j++)
            {
                map[i][j] = NEED_SCAN;
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

    bool needScan(void)
    {
        for (int dir = 0; dir < 4; dir++)
        {
            if (map[currY + dy[dir]][currX + dx[dir]] == NEED_SCAN)
                return true;
        }
        return false;
    }

    inline void mapping(int floorState, int& map, bool cleaned)
    {
        if (map == status::NEED_CLEANING)
            return;
        map = floorState;
        if (map == status::NOT_BLOCK && cleaned == false)
        {
            map = status::NEED_CLEANING;
            ++size;
        }
    }

    void mapUpdate(int floorState[3][3])
    {
        int baseY = currY - 1;
        int baseX = currX - 1;
        if (currDir == Dir::UP)
        {
            for (int y = 0, sy = 0; y <= 2; y++, sy++)
            {
                for (int x = 0, sx = 0; x <= 2; x++, sx++)
                {
                    mapping(floorState[y][x], map[baseY + sy][baseX + sx], cleaned[baseY + sy][baseX + sx]);
                }
            }
        }
        else if (currDir == Dir::LEFT)
        {
            for (int y = 0, sx = 0; y <= 2; y++, sx++)
            {
                for (int x = 0, sy = 2; x <= 2; x++, sy--)
                {
                    mapping(floorState[y][x], map[baseY + sy][baseX + sx], cleaned[baseY + sy][baseX + sx]);
                }
            }
        }
        else if (currDir == Dir::DOWN)
        {
            for (int y = 0, sy = 2; y <= 2; y++, sy--)
            {
                for (int x = 0, sx = 2; x <= 2; x++, sx--)
                {
                    mapping(floorState[y][x], map[baseY + sy][baseX + sx], cleaned[baseY + sy][baseX + sx]);
                }
            }
        }
        else if (currDir == Dir::RIGHT)
        {
            for (int y = 0, sx = 2; y <= 2; y++, sx--)
            {
                for (int x = 0, sy = 0; x <= 2; x++, sy++)
                {
                    mapping(floorState[y][x], map[baseY + sy][baseX + sx], cleaned[baseY + sy][baseX + sx]);
                }
            }
        }
    }

    void update(void)
    {
        ++visitCheck;
        min = SCORE_MAX;
        PriorityQueue pq;
        pq.push({ currY, currX, currDir, 0 });
        visit[currY][currX] = visitCheck;

        while (!pq.empty())
        {
            Coor now = pq.pop();

            if (map[now.y][now.x] == NEED_CLEANING)
            {
                min_coor = now;
                break;
            }

            for (int diff = 0; diff < 4; diff++)
            {
                int ndir = (now.dir + diff) % 4;
                int ny = now.y + dy[ndir];
                int nx = now.x + dx[ndir];
                if (map[ny][nx] != BLOCK && map[ny][nx] != NEED_SCAN && visit[ny][nx] != visitCheck)
                {
                    pq.push({ ny, nx, ndir, diff == 0 ? now.score + Cost::MOVE : now.score + Cost::TURN + Cost::MOVE });
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
            int diffDir = (path[toGo.y][toGo.x] - currDir + 4) % 4;
            if (diffDir != 0)
                turn(diffDir);
            move();
            return;
        }
        int opDir = (path[now.y][now.x] + 2) % 4;
        int ny = now.y + dy[opDir];
        int nx = now.x + dx[opDir];

        FollowPath({ ny, nx }, now);

        int diffDir = (path[toGo.y][toGo.x] - path[now.y][now.x] + 4) % 4;
        if(diffDir != 0)
            turn(diffDir);
        move();
    }
};

void init(void)
{

}

void cleanHouse(void)
{
    currY = INIT_POS, currX = INIT_POS, currDir = Dir::UP;
    int floorState[3][3];
    Robot robot;
    Coor dest;
    
    do 
    {
        if (robot.needScan() == true)
        {
            scan(floorState);
            robot.mapUpdate(floorState);
        }
        robot.update();

        if (robot.size == 0)
            break;

        dest = robot.pop();
        //path 저장된거에 따라 move 및 turn호출
        int opDir = (robot.path[dest.y][dest.x] + 2) % 4;
        int ny = dest.y + dy[opDir];
        int nx = dest.x + dx[opDir];
        robot.FollowPath({ny, nx}, dest);
        //currDir, currY, currX 갱신
        currDir = robot.path[dest.y][dest.x];
        currY = dest.y;
        currX = dest.x;
    } while (true);
}