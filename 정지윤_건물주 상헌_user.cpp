#define ROOM_SIZE 777777
#define NOTE_SIZE 100000
 
#define MAX_SIZE_2 (77769)
#define MAX_SIZE_3 (77783)
#define MAX_SIZE_5 (77776)
 
#define TYPE3_S (155540)
#define TYPE5_S (388892)
#define FIRST_MUL (32768)
#define SECOND_MUL (1024)
#define THIRD_MUL   (32)
 
#define find_lowest(A) (A & (-1 * A))
#define set_bit(bitmap,A) (bitmap &= ~(1<<A))
#define clear_bit(bitmap,A) (bitmap |= (1<<A))
 
#define QUEUE
 
//extern double search_time;
//clock_t start;
//시작지점은 그대로 고정할꺼임
//만약 시작 지점 
 
//int로 지정할꺼임.070
//각각의 구역들을 3개로 나눌거임 --> 2 77778개, 3 77777개, 5 77778개 
//random한 분포기 때문에 해당 구역대로 나누는 것은 합리적임
 
//0번 블락, 1번 블락, 2번 블락의 경우 각 chunk의 시작 위치를 나타냄 (주소값 기준)
//따라서 28, 33352, 66680의 값이 해당 블락들에 들어있음. // 고정
//모든 chunk의 모습은 동일함 따라서 bitmap의 위치의 경우 3번 4번 5번 6번 블락들에 저장함 
//first_bitmap의 경우 0번 index [0] //0~18
//second_bitmap의 경우 1번 index ~ 19번 index [1,2,3]
//third_bitmap의 경우 20번 index ~ 4000번 index [4~99]
 
/*
    int* changed_note = (int*)note;
    int* chunk = (int*)(note +changed_note[type + 1]);
    int* first_bitmap = (int*)(chunk + chunk + 1);
*/
int get_offset(unsigned int bitmap) { //최초로 발견된 0찾기.
    register int index = 0;
    while ((bitmap & (1 << index)) != 0) {
        index++;
    }
    return index;
}
 
int malloc_room(unsigned int* chunk, unsigned int* queue, int size, int mro) {
    unsigned int* bitmaps[2], offsets[2];
    bitmaps[0] = chunk;
    bitmaps[1] = chunk + 8;
    int real_offset = 0;
#ifdef QUEUE
#ifdef TC
    start = clock();
#endif
    if (queue[0] == queue[1] && queue[2] == queue[3]) {
        if (chunk[1] == 0) {
            return -1;
        }
        offsets[0] = get_offset(~bitmaps[0][0]); //return 0,1,2
        offsets[1] = get_offset(~bitmaps[1][offsets[0]]);
        unsigned int* bitmap1 = &bitmaps[1][offsets[0]];
 
        int start_offset = (offsets[0] << 17) + (offsets[1] << 12);
 
        queue[0] = start_offset;
        queue[1] = (start_offset + 4096 < mro + 1) ? start_offset + 4096 : mro + 1;
        queue[2] = 4;
        queue[3] = 4;
 
        if (queue[1] == mro + 1) {
            chunk[1] = 0;
        }
        set_bit(*bitmap1, offsets[1]);
        if (*bitmap1 == 0) {
            set_bit(bitmaps[0][0], offsets[0]);
        }
    }
#ifdef TC
    search_time += (double)(clock() - start);
#endif
    if (queue[0] < queue[1]) {
        real_offset = queue[0]++;
    }
    else if (queue[2] < queue[3]) {
        real_offset = queue[queue[2]++];
    }
#else
 
    offsets[0] = get_offset(~bitmaps[0][0]); //return 0,1,2
    offsets[1] = get_offset(~bitmaps[1][offsets[0]]);
    offsets[2] = get_offset(~bitmaps[2][(offsets[0] << 5) + offsets[1]]);
    offsets[3] = get_offset(~bitmaps[3][(offsets[0] << 10) + (offsets[1] << 5) + offsets[2]]);
    real_offset = (offsets[0] << 15) + (offsets[1] << 10) + (offsets[2] << 5) + offsets[3];
#endif
    return real_offset;
}
 
void init_note(char note[NOTE_SIZE], char rooms[ROOM_SIZE])
{
    //memset(note, 0xFF, NOTE_SIZE); //bitmap을 모두 1로 초기화
    for (int i = 80; i < 680; i++) {
        note[i] = 0xff;
    }
    long long* changed_note = (long long*)note;
    changed_note[0] = 80;
    changed_note[1] = 280;
    changed_note[2] = 480;
    changed_note[3] = (unsigned long long)rooms;
    changed_note[4] = 680;
    changed_note[5] = 30680;
    changed_note[6] = 60680;
    changed_note[7] = TYPE3_S;
    changed_note[8] = TYPE5_S;
}
 
char* sell_room(char note[NOTE_SIZE], int size) {
    unsigned int* chunk = nullptr;
    unsigned int* queue = nullptr;
    int mro;
    unsigned long long* changed_note = (unsigned long long*)note;
    int real_offset;
    switch (size) {
    case 2:
        chunk = (unsigned int*)(note + changed_note[0]);
        queue = (unsigned int*)(note + changed_note[4]);
        mro = MAX_SIZE_2;
        real_offset = malloc_room(chunk, queue, size, mro);
        if (real_offset == -1)
            return 0;
        return (char*)(changed_note[3] + real_offset * size);
    case 3:
        chunk = (unsigned int*)(note + changed_note[1]);
        queue = (unsigned int*)(note + changed_note[5]);
        mro = MAX_SIZE_3;
        real_offset = malloc_room(chunk, queue, size, mro);
        if (real_offset == -1)
            return 0;
        return (char*)(changed_note[3] + changed_note[7] + real_offset * size);
    case 5:
        chunk = (unsigned int*)(note + changed_note[2]);
        queue = (unsigned int*)(note + changed_note[6]);
        mro = MAX_SIZE_5;
        real_offset = malloc_room(chunk, queue, size, mro);
        if (real_offset == -1)
            return 0;
        return (char*)(changed_note[3] + changed_note[8] + real_offset * size);
    }
}
 
void return_room(char note[NOTE_SIZE], char* position) {
    unsigned int* chunk = nullptr;
    unsigned int* queue = nullptr;
    unsigned long long* changed_note = (unsigned long long*)note;
    int size = 0;
    unsigned int real_offset = 0;
    if (position < (char*)(changed_note[3] + changed_note[7])) {
        size = 2;
        real_offset = (unsigned int)(position - (char*)changed_note[3]) / size;
        chunk = (unsigned int*)(note + changed_note[0]);
        queue = (unsigned int*)(note + changed_note[4]);
    }
    else if (position < (char*)(changed_note[3] + changed_note[8])) {
        size = 3;
        real_offset = (unsigned int)(position - (char*)(changed_note[3] + changed_note[7])) / size;
        chunk = (unsigned int*)(note + changed_note[1]);
        queue = (unsigned int*)(note + changed_note[5]);
    }
    else {
        size = 5;
        real_offset = (unsigned int)(position - (char*)(changed_note[3] + changed_note[8])) / size;
        chunk = (unsigned int*)(note + changed_note[2]);
        queue = (unsigned int*)(note + changed_note[6]);
    }
    queue[queue[3]++] = real_offset;
}
