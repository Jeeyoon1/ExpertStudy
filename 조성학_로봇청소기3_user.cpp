extern void scanFromRobot(int floorState[3][3]);
extern int moveRobot(int mCommand);

const int LM = 64;           // 30*30 의 2*2배 크기로 잡아 사용
int Map[LM][LM];             // user가 관리하는 맵
int row, col, dir;           // user가 가정한 로봇의 현재위치와 절대방향
int state[3][3];             // scan()의 매개변수
int dr[] = { -1, 0, 1, 0 }, dc[] = { 0, -1, 0, 1 };

/// 막다른 골목에 도달한 경우 현재 위치에서 가장 가까운 청소가능 위치탐색에 사용 ///
int visited[LM][LM], vcnt;
struct Data {
	int r, c, ad, rd, path; // ad:절대방향, rd:상대방향, path:경로
}que[LM * LM];              // 막다른 골목에 도달한 경우 탐색에 사용
int fr, re;
int pathSet[LM * LM], pcnt; // 가장 가까운 청소가능한 곳으로의 경로 저장
///////////////////////////////////////////////////////////////////////////////////

void init(int N, int subTaskCount) {}
void mapUpdate() {
	scanFromRobot(state);
	int i, j, r, c;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (dir == 0)      r = row - 1 + i, c = col - 1 + j;  // 현재 방향
			else if (dir == 1) r = row - 1 + 2 - j, c = col - 1 + i;  // 왼쪽으로 90도 회전
			else if (dir == 2) r = row - 1 + 2 - i, c = col - 1 + 2 - j;  // 180동 회전
			else if (dir == 3) r = row - 1 + j, c = col - 1 + 2 - i;  // 오른쪽으로 90도 회전
			if (Map[r][c] == 3) Map[r][c] = state[i][j];  // 맵이 업데이트가 필요한 경우에만
		}
	}
}

void push(int r, int c, int ad, int rd, int np) {
	if (Map[r][c] == 1 || Map[r][c] == 3 || visited[r][c] == vcnt) return;
	visited[r][c] = vcnt;
	que[re++] = { r, c, ad, rd, np };
}

int BFS() {
	if (Map[row][col] == 0) return 1;                  // 청소 가능

	fr = re = 0, ++vcnt;                               // 큐와 방문배열 초기화
	push(row, col, dir, 0, -1);                        // 현재위치를 시드로

	for (; fr < re; ++fr) {
		Data& t = que[fr];
		if (Map[t.r][t.c] == 0) break;                 // 청소가능위치를 찾은 경우
		for (int i = 0; i < 4; ++i) {
			int d = (t.ad + i) & 3;
			push(t.r + dr[d], t.c + dc[d], d, i, fr);
		}
	}

	if (fr == re) return 0;                             // 청소할 곳이 없는 경우

	// 청소가능한 곳으로 로봇을 이동시키기
	Data& t = que[fr];
	pcnt = 0;
	for (int i = fr; i > 0; i = que[i].path)            // 경로추적
		pathSet[++pcnt] = que[i].rd;
	for (; pcnt; --pcnt)                                // main의 로봇이동
		moveRobot(pathSet[pcnt]);

	row = t.r, col = t.c, dir = t.ad;                   // user로봇 이동

	return 1;
}

void cleanHouse(int mLimitMoveCount) {
	int i, j;
	for (int i = 0; i < LM; ++i) for (int j = 0; j < LM; ++j)
		Map[i][j] = 3;                                  // 맵 초기화
	row = col = 30, dir = 0;                            // 로봇의 초기 위치와 절대 방향

	while (1) {
		Map[row][col] = 2;                              // 청소했음을 표시
		mapUpdate();
		for (i = 0; i < 4; ++i) {                       // 상대방향으로탐색(현재,왼,뒤,오른)
			int d = (dir + i) & 3, r = row + dr[d], c = col + dc[d];
			if (Map[r][c] == 0) {
				row = r, col = c, dir = d;
				moveRobot(i);
				break;                                  // 청소가능한 곳으로 로봇을 이동했으므로
			}
		}

		if (BFS() == 0)
			break; // 청소완료
	}
}
