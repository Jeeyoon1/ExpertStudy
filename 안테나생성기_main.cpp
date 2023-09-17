/// ***** main.cpp *****
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#define NUM_SUBTASK (10)
#define NUM_TASK (10)
#define MAP_SIZE (100)
#define NUM_USER (10000)
#define NUM_ANTENNA (150)
#define MAX_USER_OF_ANTENNA (100)
#define WRONG_ANSWER (1000000000)

#define ABS(x) ((x) < 0 ? -(x) : (x))


#include <stdio.h>
#include <string.h>
struct location {
	int x;
	int y;
	location() {}
	location(int a, int b) : x(a), y(b) {}

	unsigned int operator-(const location& other) const {
		return ABS(other.x - x) + ABS(other.y - y);
	}
	bool operator==(const location& other) const {
		return (other.x == x && other.y == y);
	}
};

/* -------------------------------------------user.cpp------------------------------------------------ */
//#define ABS(x) ((x) < 0 ? -(x) : (x))
//
//struct location {
//	int x;
//	int y;
//
//	location(int a, int b) : x(a), y(b) {}
//
//	unsigned int operator-(const location& other) const {
//		return ABS(other.x - x) + ABS(other.y - y);
//	}
//	bool operator==(const location& other) const {
//		return (other.x == x && other.y == y);
//	}
//};
//
//void Init(location *Antenna) {
//
//}
//void ScanAllUser(location *User_list, unsigned int* User_to_antenna, unsigned int* antenna_range) {
//
//}
extern void Init(location *Antenna);
extern void ScanAllUser(location *User_list, unsigned int* User_to_antenna, unsigned int* antenna_range);
///////////////////////////////////////////////////////////////////////////////////////////////////////

int dir_y[20] = { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0 };
int dir_x[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1 };
location user_list[NUM_USER];
location user_list_bak[NUM_USER];
unsigned int dir[NUM_USER];

location antenna_list[NUM_ANTENNA];
location antenna_list_bak[NUM_ANTENNA];

unsigned int UE2Ant[NUM_USER];
unsigned int AntRange[NUM_ANTENNA];

static unsigned long long seed = 4;

static int rand(void)
{
	seed = seed * 25214903917ULL + 11ULL;
	return (seed >> 16) & 0x3fffffff;
}

/* These constant variables will NOT be changed */

bool test(int *score) {
	for (int i = 0; i < NUM_ANTENNA; i++) {
		antenna_list[i].x = rand() % 100;
		antenna_list[i].y = rand() % 100;
		antenna_list_bak[i] = antenna_list[i];
	}
	for (int i = 0; i < NUM_USER; i++){
		user_list[i].x = rand() % 100;
		user_list[i].y = rand() % 100;
		dir[i] = rand() % 20;
	}

	Init(antenna_list_bak); //antenna 리스트 전달 
	
	for (int i = 0; i < NUM_SUBTASK; i++) {
		for (int j = 0; j < NUM_USER; j++) 
			user_list_bak[j] = user_list_bak[i];

		ScanAllUser(user_list_bak, UE2Ant, AntRange);

		
		//안테나 레인지의 범위 확인 및 안테나 조작 확인
		for (int j = 0; j < NUM_ANTENNA; j++) {
			if (AntRange[j] >= 200) 
				return false;
			if (!(antenna_list[j] == antenna_list_bak[j])) 
				return false;
		}
		//USER 조작 확인 및 연결된 list의 타당성 확인
		for (int j = 0; j < NUM_USER; j++) {
			if (!(user_list[j] == user_list_bak[j]))
				return false;
			if ((antenna_list[UE2Ant[j]] - user_list[j]) > AntRange[UE2Ant[j]])
				return false;
		}

		for (int minute = 0; minute < 5; minute++) { // 5분간 진행
			for (int j = 0; j < NUM_USER; j++) {
				if (dir_x[dir[j]] == 0 && dir_y[dir[j]] == 0)
					continue;
				if (dir_x[dir[j]] + user_list[j].x >= MAP_SIZE || dir_x[dir[j]] + user_list[j].x < 0) {
					dir[j] = (dir[j] + 10) % 20;
				}
				if (dir_y[dir[j]] + user_list[j].y >= MAP_SIZE || dir_y[dir[j]] + user_list[j].y < 0) {
					dir[j] = (dir[j] + 10) % 20;
				}
				user_list[j].x += dir_x[dir[j]];
				user_list[j].y += dir_y[dir[j]];
			}
			for (int j = 0; j < NUM_USER; j++) {
				if ((antenna_list[UE2Ant[j]] - user_list[j]) > AntRange[UE2Ant[j]])
					return false;
			}
		}
	}

	return true;
}

int main()
{
	int score = 0;
	for (int i = 0; i < NUM_TASK; i++) {
		if (!test(&score)) {
			score = WRONG_ANSWER;
			break;
		}
	}
	//아마 300만점 정도가 통과 예정 (나는 322만점 정도 받음)
	printf("SCORE is %d\n", score);
	return 0;
}

