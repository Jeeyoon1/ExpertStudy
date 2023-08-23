extern void scanFromRobot(int floorState[3][3]);
extern int moveRobot(int mCommand);

const int LM = 64;           // 30*30 �� 2*2�� ũ��� ��� ���
int Map[LM][LM];             // user�� �����ϴ� ��
int row, col, dir;           // user�� ������ �κ��� ������ġ�� �������
int state[3][3];             // scan()�� �Ű�����
int dr[] = { -1, 0, 1, 0 }, dc[] = { 0, -1, 0, 1 };

/// ���ٸ� ��� ������ ��� ���� ��ġ���� ���� ����� û�Ұ��� ��ġŽ���� ��� ///
int visited[LM][LM], vcnt;
struct Data {
	int r, c, ad, rd, path; // ad:�������, rd:������, path:���
}que[LM * LM];              // ���ٸ� ��� ������ ��� Ž���� ���
int fr, re;
int pathSet[LM * LM], pcnt; // ���� ����� û�Ұ����� �������� ��� ����
///////////////////////////////////////////////////////////////////////////////////

void init(int N, int subTaskCount) {}
void mapUpdate() {
	scanFromRobot(state);
	int i, j, r, c;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (dir == 0)      r = row - 1 + i, c = col - 1 + j;  // ���� ����
			else if (dir == 1) r = row - 1 + 2 - j, c = col - 1 + i;  // �������� 90�� ȸ��
			else if (dir == 2) r = row - 1 + 2 - i, c = col - 1 + 2 - j;  // 180�� ȸ��
			else if (dir == 3) r = row - 1 + j, c = col - 1 + 2 - i;  // ���������� 90�� ȸ��
			if (Map[r][c] == 3) Map[r][c] = state[i][j];  // ���� ������Ʈ�� �ʿ��� ��쿡��
		}
	}
}

void push(int r, int c, int ad, int rd, int np) {
	if (Map[r][c] == 1 || Map[r][c] == 3 || visited[r][c] == vcnt) return;
	visited[r][c] = vcnt;
	que[re++] = { r, c, ad, rd, np };
}

int BFS() {
	if (Map[row][col] == 0) return 1;                  // û�� ����

	fr = re = 0, ++vcnt;                               // ť�� �湮�迭 �ʱ�ȭ
	push(row, col, dir, 0, -1);                        // ������ġ�� �õ��

	for (; fr < re; ++fr) {
		Data& t = que[fr];
		if (Map[t.r][t.c] == 0) break;                 // û�Ұ�����ġ�� ã�� ���
		for (int i = 0; i < 4; ++i) {
			int d = (t.ad + i) & 3;
			push(t.r + dr[d], t.c + dc[d], d, i, fr);
		}
	}

	if (fr == re) return 0;                             // û���� ���� ���� ���

	// û�Ұ����� ������ �κ��� �̵���Ű��
	Data& t = que[fr];
	pcnt = 0;
	for (int i = fr; i > 0; i = que[i].path)            // �������
		pathSet[++pcnt] = que[i].rd;
	for (; pcnt; --pcnt)                                // main�� �κ��̵�
		moveRobot(pathSet[pcnt]);

	row = t.r, col = t.c, dir = t.ad;                   // user�κ� �̵�

	return 1;
}

void cleanHouse(int mLimitMoveCount) {
	int i, j;
	for (int i = 0; i < LM; ++i) for (int j = 0; j < LM; ++j)
		Map[i][j] = 3;                                  // �� �ʱ�ȭ
	row = col = 30, dir = 0;                            // �κ��� �ʱ� ��ġ�� ���� ����

	while (1) {
		Map[row][col] = 2;                              // û�������� ǥ��
		mapUpdate();
		for (i = 0; i < 4; ++i) {                       // ����������Ž��(����,��,��,����)
			int d = (dir + i) & 3, r = row + dr[d], c = col + dc[d];
			if (Map[r][c] == 0) {
				row = r, col = c, dir = d;
				moveRobot(i);
				break;                                  // û�Ұ����� ������ �κ��� �̵������Ƿ�
			}
		}

		if (BFS() == 0)
			break; // û�ҿϷ�
	}
}