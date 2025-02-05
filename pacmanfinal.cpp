
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <ctime>
using namespace std;
const int WIDTH = 21;
const int HEIGHT = 21;
const char PACMAN = 'C';
const char GHOST = 'G';
const char WALL = '#';
const char DOT = '.';
const char EMPTY = ' ';
struct Position {
    int x;
    int y;
};
struct Character {
    Position pos;
    int direction;
};
class Game {
private:
    char maze[HEIGHT][WIDTH];
    Character pacman;
    vector<Character> ghosts;
    int score;
    int lives;
    bool gameOver;
    int dotsEaten;
    void initializeMaze() {
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1)
                    maze[y][x] = WALL;
                else if (x % 2 == 0 && y % 2 == 0)
                    maze[y][x] = WALL;
                else
                    maze[y][x] = DOT;
            }
        }
        for (int y = 2; y < HEIGHT-2; y += 2) {
            for (int x = 2; x < WIDTH-2; x += 2) {
                maze[y][x] = DOT;
            }
        }
    }
    void drawMaze() {
        system("cls");
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                if (x == pacman.pos.x && y == pacman.pos.y)
                    cout << PACMAN;
                else {
                    bool ghostHere = false;
                    for (const auto& ghost : ghosts) {
                        if (x == ghost.pos.x && y == ghost.pos.y) {
                            cout << GHOST;
                            ghostHere = true;
                            break;
                        }
                    }
                    if (!ghostHere)
                        cout << maze[y][x];
                }
            }
            cout << endl;
        }
        cout << "Score: " << score << " Lives: " << lives << endl;
    }
    void moveCharacter(Character& character) {
        Position nextPos = character.pos;
        switch (character.direction) {
            case 0: nextPos.y--; break;
            case 1: nextPos.x++; break;
            case 2: nextPos.y++; break;
            case 3: nextPos.x--; break;
        }
        if (maze[nextPos.y][nextPos.x] != WALL) {
            character.pos = nextPos;
        }
    }
    void checkCollisions() {
        for (auto& ghost : ghosts) {
            if (pacman.pos.x == ghost.pos.x && pacman.pos.y == ghost.pos.y) {
                lives--;
                if (lives <= 0) {
                    gameOver = true;
                    cout << "Game Over! Final Score: " << score << endl;
                } else {
                    pacman.pos = {WIDTH/2, HEIGHT/2};
                    for (auto& g : ghosts) {
                        g.pos = {1, 1};
                    }
                }
            }
        }
        if (maze[pacman.pos.y][pacman.pos.x] == DOT) {
            score += 10;
            maze[pacman.pos.y][pacman.pos.x] = EMPTY;
            dotsEaten++;
            if (dotsEaten >= (WIDTH-2)*(HEIGHT-2)) {
                gameOver = true;
                cout << "You Win! Final Score: " << score << endl;
            }
        }
    }
public:
    Game() : score(0), lives(3), gameOver(false), dotsEaten(0) {
        pacman.pos = {WIDTH/2, HEIGHT/2};
        pacman.direction = 1;
        initializeMaze();
        for (int i = 0; i < 4; i++) {
            ghosts.push_back({{1, 1}, rand() % 4});
        }
    }
    void run() {
        srand(time(0));
        while (!gameOver) {
            if (_kbhit()) {
                switch (_getch()) {
                    case 72:
                        pacman.direction = 0;
                        break;
                    case 77:
                        pacman.direction = 1;
                        break;
                    case 80:
                        pacman.direction = 2;
                        break;
                    case 75:
                        pacman.direction = 3;
                        break;
                    case 'x':
                        gameOver = true;
                        break;
                }
            }
            moveCharacter(pacman);
            for (auto& ghost : ghosts) {
                if (rand() % 100 < 30) {
                    ghost.direction = rand() % 4;
                }
                moveCharacter(ghost);
            }
            checkCollisions();
            drawMaze();
            Sleep(100);
        }
    }
};
int main() {
    Game game;
    game.run();
    return 0;
}

