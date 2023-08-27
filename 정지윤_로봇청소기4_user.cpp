extern void scan(int ceilingState[3][3]);
extern int move(void);
extern void turn(int mCommand);


//#define DEBUG
#include <stdio.h>
#ifdef DEBUG
#include <stdio.h>
#endif
#define MAX_UNIT (128)
#define H_UNIT (63)
#define MAX_N (64)
#define RIGHT(dir) (((dir+3)%4))
int bfs_visit[MAX_UNIT][MAX_UNIT];
int bvc = 1;
int is_find = 0;

struct map_candidate {
	int x;
	int y;
	int dir;
};

map_candidate MC[16384][2];
int mc_pointer[2];

struct q_idx {
	int x;
	int y;
	int before;
	int depth;
};
int count = 0;
int visit[MAX_UNIT][MAX_UNIT];
// 1벽 || 청소한곳
// 0 == 빈공간


short mapp[MAX_UNIT][MAX_UNIT];

//실제 MAP 

int map[64][64][4];

int move_x[4] = { 0,-1,0,1 };
int move_y[4] = { -1,0,1,0 };

q_idx queue[1000000];
int sr = 0;
int re = 0;
int floor_state[3][3];
int x, y, dir;

void fill_map(int robot_x, int robot_y, int direction);
void init(void)
{

}
void save_map() { //mapp를 map에 저장!
	int x=0;
	int y=0;
	int flag = 0;
	for (int i = 0; i < MAX_UNIT; i++) {
		for (int j = 0; j < MAX_UNIT; j++) {
			if (mapp[i][j] != -1) {
				y = i;
				x = j;
				flag = 1;
				break;
			}
		}
		if (flag == 1)
			break;
	}
	int i1, i2, j1, j2;
	for (int i = 0; i < MAX_N; i++) {
		for (int j = 0; j < MAX_N; j++) {
			map[i][j][0] = mapp[i + y][j + x];
		}
	}
	for (i1 = 0, j2 = MAX_N; i1 < MAX_N; i1++, j2--) {
		for (j1 = 0, i2 = 0; j1 < MAX_N; j1++, i2++) {
			map[i1][j1][1] = map[i2][j2][0];
		}
	}
	for (i1 = 0,j2 = MAX_N-1; i1 < MAX_N; i1++, j2--) {
		for (j1 = 0, i2 = 0; j1 < MAX_N; j1++, i2++) {
			map[i1][j1][1] = map[i2][j2][0];
		}
	}
	for (i1 = 0, i2 = MAX_N-1; i1 < MAX_N; i1++, i2--) {
		for (j1 = 0, j2 = MAX_N-1; j1 < MAX_N; j1++, j2--) {
			map[i1][j1][2] = map[i2][j2][0];
		}
	}
	for (i1 = 0, j2 = 0; i1 < MAX_N; i1++, j2++) {
		for (j1 = 0, i2 = MAX_N-1; j1 < MAX_N; j1++, i2--) {
			map[i1][j1][3] = map[i2][j2][0];
		}
	}
#ifdef DEBUG
	for (int k = 0; k < 4; k++) {
		for (int i = 0; i < MAX_N; i++) {
			for (int j = 0; j < MAX_N; j++) {
				printf("%d ", map[i][j][k]);
			}
			printf("\n");
		}
		printf("\n");
	}
#endif
}
inline int distance(int x1, int x2, int y1, int y2) {
	return ((x1 > x2) ? (x1 - x2) : (x2 - x1)) + ((y1 > y2) ? (y1 - y2) : (y2 - y1));
}
inline void move_to(int to_x, int to_y) {
	for (int i = 0; i < 4; i++) {
		if (to_x == x + move_x[i] && to_y == y + move_y[i]) {
			//to_dir = i;
			if (dir != i) {
				turn((i - dir + 4) % 4);
				dir = i;
			}
			move();
			x = x + move_x[i];
			y = y + move_y[i];
			return;
		}
	}
#ifdef DEBUG
	printf("fuck\n");
#endif
}
void follow_path(int re) {
	int x_arr[64 * 64];
	int y_arr[64 * 64];
	int depth = queue[re].depth;
	while (re > -1) {
		q_idx qi = queue[re];
		x_arr[qi.depth] = qi.x;
		y_arr[qi.depth] = qi.y;
		re = qi.before;
	}
	//x,y 부터 시작해서 가는 방향들이 다 나와있음.
	for (int i = 0; i <= depth; i++) {
		move_to(x_arr[i], y_arr[i]);
	}
	visit[y][x] = 1;
	if (mapp[y + move_y[0]][x + move_x[0]] == -1 || mapp[y + move_y[1]][x + move_x[1]] == -1
		|| mapp[y + move_y[2]][x + move_x[2]] == -1 || mapp[y + move_y[3]][x + move_x[3]] == -1) {
		scan(floor_state);
		fill_map(x, y, dir);
	}
	if (visit[y + move_y[(dir + 3) % 4]][x + move_x[(dir + 3) % 4]] == 1) //오른쪽 확인
		return;
	for (int i = 0; i < 3; i++) {
		if (visit[y + move_y[(dir + i) % 4]][x + move_x[(dir + i) % 4]] == 1) { //앞 뒤 왼쪽 확인 후 오른쪽에 벽이 있도록 돌거임
			turn(i + 1);
			dir = (dir + i + 1) % 4;
			return;
		}
	}

}
#ifdef DEUBUG
void print_map() {
	for (int i = 0; i < MAX_UNIT; i++) {
		for (int j = 0; j < MAX_UNIT; j++) {
			printf("%d ", mapp[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	for (int i = 0; i < MAX_UNIT; i++) {
		for (int j = 0; j < MAX_UNIT; j++) {
			printf("%d ", visit[i][j]);
		}
		printf("\n");
	}
}
#endif
bool go_nearest_uncleaned() {
	sr = 0;
	re = 0;
	q_idx qi;	//int before;
	int i;

	bfs_visit[y][x] = bvc++;
	for (i = 0; i < 4; i++) {
		if (mapp[y + move_y[i]][x + move_x[i]] == 0) {
			bfs_visit[y + move_y[i]][x + move_x[i]] = bvc;
			queue[sr].x = x + move_x[i];
			queue[sr].y = y + move_y[i];
			queue[sr].depth = 0;
			queue[sr++].before = -1;
		}
	}
	while (sr > re) {
		qi = queue[re++];
		if (visit[qi.y][qi.x] == 0) {
			follow_path(re - 1);
			return true;
		}
		for (i = 0; i < 4; i++) {
			if (bfs_visit[qi.y + move_y[i]][qi.x + move_x[i]] != bvc && mapp[qi.y + move_y[i]][qi.x + move_x[i]] == 0) {
				bfs_visit[qi.y + move_y[i]][qi.x + move_x[i]] = bvc;
				queue[sr].x = qi.x + move_x[i];
				queue[sr].y = qi.y + move_y[i];
				queue[sr].depth = qi.depth + 1;
				queue[sr++].before = re - 1;
			}
		}
	}
	return false;
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
					visit[y][x] = 1;
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
					visit[y][x] = 1;
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
					visit[y][x] = 1;
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
					visit[y][x] = 1;
				}
			}
		}
	}
}
//방문한 곳을 벽처럼 취급하기 위한 함수
void fill_visit() {
	for (int i = 0; i < MAX_UNIT; i++) {
		for (int j = 0; j < MAX_UNIT; j++) {
			if (visit[i][j] == 1) {
				visit[i][j] = 1;
			}
		}
	}
}
void fill_mapp_by_map(int x, int y, int dir) {
	for (int i = 0; i < MAX_N; i++) {
		for (int j = 0; j < MAX_N; j++) {
			if (mapp[i + y][j + x] == -1) {
				mapp[i + y][j + x] = map[i][j][dir];
				if (map[i][j][dir] == 1) {
					visit[i + y][j + x] = 1;
				}
			}
			else {
				if (mapp[i + y][j + x] != map[i][j][dir]) {
					printf("1234\n");
				}
			}
		}
	}
}
bool check_map() { // 후보군 뒤져가면서 map 뒤져보고 완벽하게 일치하는게 1개만 있다면 return true; 아니면 후보군 추리고 return false;
	if (mc_pointer[0] == -1 && mc_pointer[1] == -1) {// 처음 시작인 경우
		mc_pointer[0] = 0; mc_pointer[1] = 0;
		for (int k = 0; k < 4; k++) {
			for (int i1 = 0; i1 <= H_UNIT; i1++) {
				for (int j1 = 0; j1 <= H_UNIT; j1++) {
					int flag = 0;
					for (int i = 0; i < MAX_N; i++) {
						for (int j = 0; j < MAX_N; j++) {
							if (mapp[i + i1][j + j1] == -1 || mapp[i + i1][j + j1] == map[i][j][k])
								continue;
							if (mapp[i + i1][j + j1] != map[i][j][k]) {
								flag = 1;
								break;
							}
						}
						if (flag == 1) {
							break;
						}
					}
					if (flag == 0) {
						MC[mc_pointer[0]][mc_pointer[1]].x = j1;
						MC[mc_pointer[0]][mc_pointer[1]].y = i1;
						MC[mc_pointer[0]++][mc_pointer[1]].dir = k;
					}
				}
			}
		}
		if (mc_pointer[0] == 1) {
			//printf("mc_pointer[0] is %d\n", mc_pointer[0]);
			fill_mapp_by_map(MC[0][mc_pointer[1]].x, MC[0][mc_pointer[1]].y, MC[0][mc_pointer[1]].dir);
			return true;
		}
		//printf("mc_pointer[0] is %d\n", mc_pointer[0]);
		return false;
	}
	int mp = mc_pointer[1];
	int cnt = mc_pointer[0];
	mc_pointer[1] = 1 - mc_pointer[1];
	mc_pointer[0] = 0;
	int i1, j1, dir;
	for (int k = 0; k < cnt; k++) {
		i1 = MC[k][mp].y; j1 = MC[k][mp].x; dir = MC[k][mp].dir;
		int flag = 0;
		for (int i = 0; i < MAX_N; i++) {
			for (int j = 0; j < MAX_N; j++) {
				if (mapp[i + i1][j + j1] == -1 || mapp[i + i1][j + j1] == map[i][j][dir])
					continue;
				if (mapp[i + i1][j + j1] != map[i][j][dir]) {
					flag = 1;
					break;
				}
			}
			if (flag == 1) {
				break;
			}
		}
		if (flag == 0) {
			MC[mc_pointer[0]][mc_pointer[1]].x = j1;
			MC[mc_pointer[0]][mc_pointer[1]].y = i1;
			MC[mc_pointer[0]++][mc_pointer[1]].dir = dir;
			//if (mc_pointer[0] == 1) {
			//	printf("push %d %d %d\n", MC[0][mc_pointer[1]].x, MC[0][mc_pointer[1]].y, MC[0][mc_pointer[1]].dir);
		//	}
		}
	}
	if (mc_pointer[0] == 0) {
		printf(",.,,?\n");
	}
	if (mc_pointer[0] == 1) {
	//	printf("mc_pointer[0] is %d %d %d\n", MC[0][mc_pointer[1]].x, MC[0][mc_pointer[1]].y, MC[0][mc_pointer[1]].dir);
		fill_mapp_by_map(MC[0][mc_pointer[1]].x, MC[0][mc_pointer[1]].y, MC[0][mc_pointer[1]].dir);
		return true;
	}
	//printf("mc_pointer[0] is %d\n", mc_pointer[0]);
	return false;
}
void wall_tracking() {
	int start_x = x;
	int start_y = y;
	int flag = 0;
	int coo = 0;
	while (1) {
		if (is_find == 0 && coo % 10 == 0) {
			coo++;
			if (check_map()) {
				is_find = 1;
			}
		}
		else if(is_find == 0) {
			coo++;
		}
		if (flag == 1 && start_x == x && start_y == y) {
			//fill_visit();
			break;
		}
		//내가 확인 할 곳은 왼쪽, 앞, 앞에서 오른쪽 데각선. 이중 하나라도 밝혀져 있지 않다면 Scan으로 밝히고 간다.
		if (mapp[y + move_y[dir]][x + move_x[dir]] == -1
			|| mapp[y + move_y[dir] + move_y[(dir + 3) % 4]][x + move_x[dir] + move_x[(dir + 3) % 4]] == -1
			|| mapp[y + move_y[(dir + 1) % 4]][x + move_x[(dir + 1) % 4]] == -1
			|| mapp[y + move_y[(dir + 2) % 4]][x + move_x[(dir + 2) % 4]] == -1
			|| mapp[y + move_y[(dir + 3) % 4]][x + move_x[(dir + 3) % 4]] == -1)
		{
			scan(floor_state);
			fill_map(x, y, dir);
		}
		//앞과 오른쪽길이 열려있으면 우측 벽면을 타기 위한 벽타기
		if (visit[y + move_y[dir]][x + move_x[dir]] == 0 && visit[y + move_y[dir] + move_y[(dir + 3) % 4]][x + move_x[dir] + move_x[(dir + 3) % 4]] == 0) {
			move();
			turn(3);
			move();
			visit[y + move_y[dir]][x + move_x[dir]] = 1;
			x += (move_x[dir] + move_x[(dir + 3) % 4]);
			y += (move_y[dir] + move_y[(dir + 3) % 4]);
			visit[y][x] = 1;
			dir = (dir + 3) % 4;
			flag = 1;
		}
		//앞은 열려있고 우측 길은 열려있지 않다면 직진
		else if (visit[y + move_y[dir]][x + move_x[dir]] == 0) {
			move();
			x += move_x[dir];
			y += move_y[dir];
			visit[y][x] = 1;
			flag = 1;
		}
		// 앞이 막혀있다면 왼쪽을 확인해보고, 왼쪽이 막혀있다면 한바퀴 회전, 막혀있지 않다면 왼쪽으로 회전
		else if (visit[y + move_y[dir]][x + move_x[dir]] != 0) {
			if (visit[y + move_y[(dir + 1) % 4]][x + move_x[(dir + 1) % 4]] == 0) { //막혀있다면
				turn(1);
				dir = (dir + 1) % 4;
			}
			else if(visit[y+move_y[(dir+2)%4]][x+move_x[(dir+2)%4]] == 0) { // 뒤가 막혀있다면
				turn(2);
				dir = (dir + 2) % 4;
			}
			else {
				//fill_visit();
				break;
			}
		}

	}
}
void cleanHouse(void)
{
	//printf("CH\n");
	is_find = 0;
	mc_pointer[0] = -1;
	mc_pointer[1] = -1;
	if (count % 10 == 0){
		is_find = 1;
	}
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
		scan(floor_state);
		fill_map(x, y, dir);

		if (floor_state[0][1] == 0) {
			move();
			x = x + move_x[0];
			y = y + move_y[0];
		}
		if (floor_state[0][1] == 1) {
			/*if (is_find == 0 && check_map()) {
				is_find = 1;
			}*/
			turn(1);
			dir = 1;
			wall_tracking();
			break;
		}
	}
	while (1) {
	if (!go_nearest_uncleaned()) {
		if (count % 10 == 0) {
			save_map();
		}
		count++;
		return;
	}
#ifdef DEBUG
		print_map();
#endif
		wall_tracking();
#ifdef DEBUG
		//print_map();
#endif		
	}
}
