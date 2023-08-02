#define ROOM_SIZE 777777
#define NOTE_SIZE 100000
#define ROOM_TYPES 3

typedef unsigned int uint;
typedef unsigned long long ull;
typedef unsigned char uchar;

#define TWO_BASE_BLOCK_NOTE     0
#define THREE_BASE_BLOCK_NOTE   8
#define FIVE_BASE_BLOCK_NOTE    16

#define ROOMS_ADDR_NOTE         24

#define TWO_START_FLOOR         0
#define THREE_START_FLOOR       155556
#define FIVE_START_FLOOR        388887

#define TWO_END_FLOOR           155555
#define THREE_END_FLOOR         388886
#define FIVE_END_FLOOR          777776

constexpr uint TWO_FREE_COUNT = (TWO_END_FLOOR - TWO_START_FLOOR + 1) / 2;
constexpr uint THREE_FREE_COUNT = (THREE_END_FLOOR - THREE_START_FLOOR + 1) / 3;
constexpr uint FIVE_FREE_COUNT = (FIVE_END_FLOOR - FIVE_START_FLOOR + 1) / 5;

struct node
{
    char* roomAddress;
    uint freeCount;
    node* next;
};

void setAddr(char* note, ull ptr)
{
    for (register uint i = 0; i < 8; i++)
    {
        note[i] = static_cast<char>(ptr >> (56 - (i << 3)));
    }
}

template <typename T>
T* getAddr(char* note)
{
    ull ret = 0;
    for (uint i = 0; i < 8; i++)
    {
        ret |= static_cast<ull>(static_cast<uchar>(note[i])) << (56 - (i << 3));
    }
    return reinterpret_cast<T*>(ret);
}

void _init_note(char note[NOTE_SIZE], char rooms[ROOM_SIZE], uint freeCount, uint startFloor, uint baseBlockNote)
{
    node* baseBlock = new node;
    node* initBlock = new node;

    initBlock->freeCount = freeCount;
    initBlock->roomAddress = rooms + startFloor;
    initBlock->next = nullptr;

    baseBlock->next = initBlock;

    setAddr(note + baseBlockNote, reinterpret_cast<ull>(baseBlock));
}

void init_note(char note[NOTE_SIZE], char rooms[ROOM_SIZE])
{
    setAddr(note + ROOMS_ADDR_NOTE, reinterpret_cast<ull>(rooms));
    _init_note(note, rooms, TWO_FREE_COUNT, TWO_START_FLOOR, TWO_BASE_BLOCK_NOTE);
    _init_note(note, rooms, THREE_FREE_COUNT, THREE_START_FLOOR, THREE_BASE_BLOCK_NOTE);
    _init_note(note, rooms, FIVE_FREE_COUNT, FIVE_START_FLOOR, FIVE_BASE_BLOCK_NOTE);
}

char* _sell_room(char note[NOTE_SIZE], int size, uint baseBlockNote)
{
    node* p = getAddr<node>(note + baseBlockNote);

    if (p->next == nullptr)
    {
        return nullptr;
    }

    else if (p->next->freeCount == 1)
    {
        node* allocBlock = p->next;
        p->next = allocBlock->next;

        char* ret = allocBlock->roomAddress;
        delete allocBlock;

        return ret;
    }

    else
    {
        char* ret = p->next->roomAddress;
        p->next->roomAddress += size;
        p->next->freeCount -= 1;

        return ret;
    }
}

char* sell_room(char note[NOTE_SIZE], int size)
{
    switch (size)
    {
    case 2:
        return _sell_room(note, size, TWO_BASE_BLOCK_NOTE);
    case 3:
        return _sell_room(note, size, THREE_BASE_BLOCK_NOTE);
    case 5:
        return _sell_room(note, size, FIVE_BASE_BLOCK_NOTE);
    default:
        return nullptr;
    }
}

void _return_room(char note[NOTE_SIZE], char* position, uint size, uint baseBlockNote)
{
    node* p = getAddr<node>(note + baseBlockNote);

    node* newBlock = new node;
    newBlock->freeCount = 1;
    newBlock->roomAddress = position;
    newBlock->next = p->next;

    p->next = newBlock;
    return;
}

void return_room(char note[NOTE_SIZE], char* position)
{
    static char* roomsAddr = getAddr<char>(note + ROOMS_ADDR_NOTE);
    uint floor = position - roomsAddr;

    if (floor >= TWO_START_FLOOR && floor < TWO_END_FLOOR)
    {
        _return_room(note, position, 2, TWO_BASE_BLOCK_NOTE);
        return;
    }
    else if (floor >= THREE_START_FLOOR && floor < THREE_END_FLOOR)
    {
        _return_room(note, position, 3, THREE_BASE_BLOCK_NOTE);
        return;
    }
    else
    {
        _return_room(note, position, 5, FIVE_BASE_BLOCK_NOTE);
        return;
    }
}