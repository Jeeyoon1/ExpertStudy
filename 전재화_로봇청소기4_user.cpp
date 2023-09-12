extern void scan(int floorState[3][3]);
extern int move(void);
extern void turn(int mCommand);

/////////////// Data Structure //////////////////
struct Coor
{
    int y, x, dir, score;
};

struct Diff
{
    int dy, dx, status;
};

struct Candidate
{
    int y, x, dir;
    bool valid;
};

////////////////////////////////////////////////

//////////////////// API ///////////////////////
static int constexpr QueueMax = 64 * 64 * 4 + 10;

template <typename T>
struct Queue
{
    T data[QueueMax];
    int front, rear;

    Queue() : front(0), rear(0)
    {

    }

    void init(void)
    {
        front = 0, rear = 0;
    }

    bool empty(void)
    {
        return front == rear;
    }

    int size(void)
    {
        return rear;
    }

    void push(T src)
    {
        data[rear++] = src;
    }

    T pop(void)
    {
        return data[front++];
    }
};

struct PriorityQueue
{
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
/////////////////////////////////////////////////

///////////// global variable ///////////////////
#define MAX 150
#define N 64
#define INIT_POS 75
#define VALID true
#define INVALID false
#define THRESHOLD 5

enum Dir { UP, LEFT, DOWN, RIGHT }; //4로 나눈 나머지 연산을 통해 방향 전환 후 진행방향을 알 수 있음
enum Cost { SCAN = 20, TURN = 15, MOVE = 10 };
enum Status { NOT_BLOCK = 0, BLOCK, NEED_SCAN };

int subTask;
int currDir, currY, currX;
int dy[4] = { -1,0,+1,0 };
int dx[4] = { 0,-1,0,+1 };
int map[MAX][MAX];
int compMap[MAX][MAX];
long long int visit[MAX][MAX];
long long int visitCheck;
bool cleaned[MAX][MAX];
int path[MAX][MAX];
int size;
Coor min_coor;
int rotateMap[4][MAX][MAX];
Queue<Candidate> candidateQ;
int cddQSize;
/////////////////////////////////////////////////

void initMap(void)
{
    size = 0;
    for (int i = 0; i < MAX; i++)
    {
        for (int j = 0; j < MAX; j++)
        {
            map[i][j] = NEED_SCAN;
            cleaned[i][j] = false;
        }
    }
    cleaned[INIT_POS][INIT_POS] = true;
}

void initCompMap(void)
{
    size = 0;
    for (int i = 0; i < MAX; i++)
    {
        for (int j = 0; j < MAX; j++)
        {
            compMap[i][j] = NEED_SCAN;   
            cleaned[i][j] = false;
        }
    }
    cleaned[INIT_POS][INIT_POS] = true;
}

Coor pop(void)
{
    cleaned[min_coor.y][min_coor.x] = true;
    --size;
    return min_coor;
}

bool needScan(int src[][MAX])
{
    for (int y = currY - 1; y <= currY + 1; ++y)
        for (int x = currX - 1; x <= currX + 1; ++x)
            if (src[y][x] == NEED_SCAN)
                return true;

    return false;
}

inline void mapping(int floorState, int& map, bool cleaned)
{
    if (map == NOT_BLOCK && cleaned == false) //이미 카운팅 한 경우
        return;
    map = floorState;
    if (map == NOT_BLOCK && cleaned == false)  
        ++size;  
}

void mapUpdate(int floorState[3][3], int dest[][MAX])
{
    int baseY = currY - 1;
    int baseX = currX - 1;
    switch (currDir)
    {
    case UP:
        for (int y = 0, sy = 0; y <= 2; y++, sy++)
            for (int x = 0, sx = 0; x <= 2; x++, sx++)
                mapping(floorState[y][x], dest[baseY + sy][baseX + sx], cleaned[baseY + sy][baseX + sx]);
        break;
    case LEFT:
        for (int y = 0, sx = 0; y <= 2; y++, sx++)
            for (int x = 0, sy = 2; x <= 2; x++, sy--)
                mapping(floorState[y][x], dest[baseY + sy][baseX + sx], cleaned[baseY + sy][baseX + sx]);
        break;
    case DOWN:
        for (int y = 0, sy = 2; y <= 2; y++, sy--)
            for (int x = 0, sx = 2; x <= 2; x++, sx--)
                mapping(floorState[y][x], dest[baseY + sy][baseX + sx], cleaned[baseY + sy][baseX + sx]);
        break;
    case RIGHT:
        for (int y = 0, sx = 2; y <= 2; y++, sx--)
            for (int x = 0, sy = 0; x <= 2; x++, sy++)
                mapping(floorState[y][x], dest[baseY + sy][baseX + sx], cleaned[baseY + sy][baseX + sx]);
        break;
    }
}

void update(int src[][MAX])
{
    ++visitCheck;
    PriorityQueue pq;
    pq.push({ currY, currX, currDir, 0 });
    visit[currY][currX] = visitCheck;
    path[currY][currX] = currDir;

    while (!pq.empty())
    {
        Coor now = pq.pop();

        if (src[now.y][now.x] == NOT_BLOCK && cleaned[now.y][now.x] == false)
        {
            min_coor = now;
            break;
        }

        for (int diff = 0; diff < 4; diff++)
        {
            int ndir = (now.dir + diff) % 4;
            int ny = now.y + dy[ndir];
            int nx = now.x + dx[ndir];
            if (src[ny][nx] == NOT_BLOCK && visit[ny][nx] != visitCheck)
            {
                pq.push({ ny, nx, ndir, diff == 0 ? now.score + Cost::MOVE : now.score + Cost::TURN + Cost::MOVE });
                visit[ny][nx] = visitCheck;
                path[ny][nx] = ndir;
            } 
        }
    }
}

void followPath(Coor now)
{
    if (now.y == currY && now.x == currX)
        return;

    int opDir = (path[now.y][now.x] + 2) % 4;
    int ny = now.y + dy[opDir];
    int nx = now.x + dx[opDir];

    followPath({ ny, nx });

    int diffDir = (path[now.y][now.x] - path[ny][nx] + 4) % 4;
    if(diffDir != 0)
        turn(diffDir);
    move();
}

bool inline isValidRange(int y, int x)
{
    return y >= 0 && y < MAX && x >= 0 && x < MAX;
}

bool isValid(Queue<Diff>& q, int y, int x, int dir)
{
    int ny, nx;
    for (int idx = 0; idx < q.size(); idx++)
    {
        Diff now = q.data[idx];
        ny = y + now.dy;
        nx = x + now.dx;
        if (isValidRange(ny, nx) == false || rotateMap[dir][ny][nx] == NEED_SCAN || rotateMap[dir][ny][nx] != now.status)
            return false;
    }
    return true;
}

void copyMap(int src[][MAX], int pivotY, int pivotX)
{
    int dy = INIT_POS - pivotY;
    int dx = INIT_POS - pivotX;
    for (int y = 0; y < MAX; y++)
    {
        for (int x = 0; x < MAX; x++)
        {
            if (src[y][x] == NEED_SCAN || isValidRange(y + dy, x + dx) == false)
                continue;
            compMap[y + dy][x + dx] = src[y][x];
            if (compMap[y + dy][x + dx] == NOT_BLOCK && cleaned[y + dy][x + dx] == false)
                ++size;
        }
    }
}

bool detectStatus(bool isFirst)
{
    ++visitCheck;
    Queue<Diff> diffQ;
    diffQ.push({ 0, 0, compMap[INIT_POS][INIT_POS] });
    visit[INIT_POS][INIT_POS] = visitCheck;

    while (!diffQ.empty())
    {
        Diff now = diffQ.pop();
        int nowY = INIT_POS + now.dy;
        int nowX = INIT_POS + now.dx;

        for (int dir = 0; dir < 4; dir++)
        {
            int ny = nowY + dy[dir];
            int nx = nowX + dx[dir];
            if (isValidRange(ny, nx) == true && compMap[ny][nx] != NEED_SCAN && visit[ny][nx] != visitCheck)
            {
                int nstatus = compMap[ny][nx];              
                diffQ.push({ now.dy + dy[dir], now.dx + dx[dir], nstatus });
                visit[ny][nx] = visitCheck;
            }
        }
    }
    // 첫번째 detect status 면 map과 diff queue를 비교하면서, 가능한 후보군 개수 추리기
    if (isFirst == true)
    {
        candidateQ.init();
        cddQSize = 0;
        for (int dir = 0; dir < 4; dir++)
        {
            for (int i = 0; i < MAX; i++)
            {
                for (int j = 0; j < MAX; j++)
                {
                    if (rotateMap[dir][i][j] != NEED_SCAN && isValid(diffQ, i, j, dir))
                    {
                        candidateQ.push({ i,j,dir,VALID });
                        ++cddQSize;
                    }

                }
            }
        }
       
    }
    else
    {
        for (int idx = 0; idx < candidateQ.size(); idx++)
        {
            Candidate& now = candidateQ.data[idx];
            if (now.valid == INVALID)
                continue;
            if (isValid(diffQ, now.y, now.x, now.dir) == false)
            {
                now.valid = INVALID;
                --cddQSize;
            }
        }
    }

    if (cddQSize == 1)
    {
        Candidate ans;
        for (int idx = 0; idx < candidateQ.size(); idx++)
        {
            Candidate& now = candidateQ.data[idx];
            if (now.valid == VALID)
            {
                ans = now;
                break;
            }
        }        
        copyMap(rotateMap[ans.dir], ans.y, ans.x);
        return true;
    }
    else
        return false;
}


void init(void)
{
    //do nothing
}

void makeMap(void)
{
    int floorState[3][3];
    Coor dest;

    do
    {
        if (needScan(map) == true)
        {
            scan(floorState);
            mapUpdate(floorState, map);
        }

        if (size == 0)
            break;

        update(map);

        dest = pop();

        followPath(dest);

        currDir = path[dest.y][dest.x];
        currY = dest.y;
        currX = dest.x;
    } while (true);   

    //make rotateMap
    for (int y = 0, sy = 0; y < MAX; ++y, ++sy)
        for (int x = 0, sx = 0; x < MAX; ++x, ++sx)
            rotateMap[UP][sy][sx] = map[y][x];

    for (int y = 0, sx = MAX - 1; y < MAX; ++y, --sx)
        for (int x = 0, sy = 0; x < MAX; ++x, ++sy)
            rotateMap[LEFT][sy][sx] = map[y][x];

    for (int y = 0, sy = MAX - 1; y < MAX; ++y, --sy)
        for (int x = 0, sx = MAX - 1; x < MAX; ++x, --sx)
            rotateMap[DOWN][sy][sx] = map[y][x];

    for (int y = 0, sx = 0; y < MAX; ++y, ++sx)
        for (int x = 0, sy = MAX - 1; x < MAX; ++x, --sy)
            rotateMap[RIGHT][sy][sx] = map[y][x];
}

void reuseMap(void)
{
    int floorState[3][3];
    Coor dest;
    bool isFirst = true;

    do
    {
        int num = 0;
        while (num++ < THRESHOLD)
        {
            if (needScan(compMap) == true)
            {
                scan(floorState);
                mapUpdate(floorState, compMap);
            }

            if (size == 0)
                break;

            update(compMap);

            dest = pop();

            followPath(dest);

            currDir = path[dest.y][dest.x];
            currY = dest.y;
            currX = dest.x;
        }

        if (detectStatus(isFirst) == true)
            break;
                
        isFirst = false;
    } while (true);

    while (size != 0)
    {
        update(compMap);

        dest = pop();

        followPath(dest);

        currDir = path[dest.y][dest.x];
        currY = dest.y;
        currX = dest.x;
    }
}

void cleanHouse(void)
{
    currY = INIT_POS, currX = INIT_POS, currDir = Dir::UP;

    if (subTask % 10 == 0)
    {
        initMap();
        makeMap();
    }
    else
    {
        initCompMap();
        reuseMap();
    }
    
    ++subTask;
}