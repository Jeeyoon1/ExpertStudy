#include <iostream>
#include <fstream>
#include <random>
#include <ctime>

#define MIN_WALLS 50
#define MAX_WALLS 100
#define MAX_WALL_LENGTH 20

const int MAP_SIZE = 64;
const int WALL = 1;
const int SPACE = 0;

enum Direction { UP, RIGHT, DOWN, LEFT };

std::mt19937 gen(static_cast<unsigned int>(time(0)));
std::uniform_int_distribution<int> dirDist(0, 3);
std::uniform_int_distribution<int> lenDist(1, MAX_WALL_LENGTH);
std::uniform_int_distribution<int> posDist(1, MAP_SIZE - 2);
std::uniform_int_distribution<int> numWallsDist(MIN_WALLS, MAX_WALLS);

// Function to initialize the map with 1s on the edges and 0s inside
void initializeMap(int map[MAP_SIZE][MAP_SIZE]) {
    for (int i = 0; i < MAP_SIZE; ++i) {
        for (int j = 0; j < MAP_SIZE; ++j) {
            if (i == 0 || i == MAP_SIZE - 1 || j == 0 || j == MAP_SIZE - 1) {
                map[i][j] = WALL;
            }
            else {
                map[i][j] = SPACE;
            }
        }
    }
}

// Function to check if the given position is within the map and not a wall (1)
bool isValid(int x, int y, int map[MAP_SIZE][MAP_SIZE]) {
    return (x >= 0 && x < MAP_SIZE && y >= 0 && y < MAP_SIZE && map[x][y] != WALL);
}

// DFS to check if the 0s are connected and not disconnected by walls
void dfs(int x, int y, int map[MAP_SIZE][MAP_SIZE], bool visited[MAP_SIZE][MAP_SIZE]) {
    if (!isValid(x, y, map) || visited[x][y])
        return;

    visited[x][y] = true;

    dfs(x + 1, y, map, visited);
    dfs(x - 1, y, map, visited);
    dfs(x, y + 1, map, visited);
    dfs(x, y - 1, map, visited);
}

// Function to check if all 0s are connected
bool isConnected(int map[MAP_SIZE][MAP_SIZE]) {
    bool visited[MAP_SIZE][MAP_SIZE] = { false };

    // Find the first unvisited 0
    int startX = -1, startY = -1;
    for (int i = 1; i < MAP_SIZE - 1; ++i) {
        for (int j = 1; j < MAP_SIZE - 1; ++j) {
            if (map[i][j] == SPACE) {
                startX = i;
                startY = j;
                break;
            }
        }
        if (startX != -1) break;
    }

    // If no 0 is found, it means the map is all walls
    if (startX == -1 || startY == -1) return true;

    dfs(startX, startY, map, visited);

    // Check if all 0s are visited
    for (int i = 1; i < MAP_SIZE - 1; ++i) {
        for (int j = 1; j < MAP_SIZE - 1; ++j) {
            if (map[i][j] == SPACE && !visited[i][j])
                return false;
        }
    }

    return true;
}

// Function to copy the contents of one map to another
void copyMap(int destination[MAP_SIZE][MAP_SIZE], int source[MAP_SIZE][MAP_SIZE]) {
    for (int i = 0; i < MAP_SIZE; ++i) {
        for (int j = 0; j < MAP_SIZE; ++j) {
            destination[i][j] = source[i][j];
        }
    }
}

// Function to generate a random wall from a given position and direction
bool generateWall(int map[MAP_SIZE][MAP_SIZE]) {
    int x = posDist(gen);
    int y = posDist(gen);
    Direction dir = static_cast<Direction>(dirDist(gen));

    int len = lenDist(gen);
    int tempMap[MAP_SIZE][MAP_SIZE];

    copyMap(tempMap, map);

    for (int i = 0; i < len; ++i) {
        if (isValid(x, y, tempMap)) {
            tempMap[x][y] = WALL;
        }
        else {
            return false;
        }

        switch (dir) {
        case UP:    --x; break;
        case RIGHT: ++y; break;
        case DOWN:  ++x; break;
        case LEFT:  --y; break;
        }
    }

    if (isConnected(tempMap)) {
        copyMap(map, tempMap);
        return true;
    }
    else {
        return false;
    }
}

// Function to generate a random map with random walls
void generateRandomMap(int map[MAP_SIZE][MAP_SIZE]) {
    initializeMap(map);

    int numWalls = numWallsDist(gen);

    while (numWalls > 0) {
        if (generateWall(map)) {
            numWalls--;
        }
    }
}

// Function to save the map to a file
void saveMapToFile(int map[MAP_SIZE][MAP_SIZE], std::ofstream& outFile) {
    for (int i = 0; i < MAP_SIZE; ++i) {
        for (int j = 0; j < MAP_SIZE; ++j) {
            outFile << map[i][j] << " ";
        }
        outFile << "\n";
    }
    outFile << "\n";
}

int main() {
    // Create and open the output file
    std::ofstream outFile("robotCleaner4_input.txt");
    if (!outFile) {
        std::cerr << "Error creating output file.";
        return 1;
    }

    int map[MAP_SIZE][MAP_SIZE];

    // Generate 10 random maps and save them to the file
    for (int i = 0; i < 10; ++i) {
        generateRandomMap(map);
        saveMapToFile(map, outFile);
    }

    // Close the output file
    outFile.close();

    std::cout << "Maps generated and saved to robotCleaner4_input.txt" << std::endl;
    return 0;
}
