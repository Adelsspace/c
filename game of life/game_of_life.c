#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

#define COLS 80
#define ROWS 25

void startGameOfLife();
void inputGrid(int grid[ROWS][COLS]);
void printGrid(int grid[ROWS][COLS]);
void updateGrid(int grid[ROWS][COLS]);
int countNeighbors(int row, int col, int grid[ROWS][COLS]);
int edges_rows(int i);
int edges_cols(int j);
int setTime(char v);
void checkStopLife(int grid[ROWS][COLS], int *flagToEnd, int *detected_no_life);

int main() {
    startGameOfLife();
    return 0;
}

void startGameOfLife() {
    int grid[ROWS][COLS];
    inputGrid(grid);
    int flagToEnd = 1;
    if (freopen("/dev/tty", "r", stdin)) {
        initscr();
    }
    noecho();
    nodelay(stdscr, 1);
    int time = 700000;
    int detected_no_life = 0;

    while (flagToEnd) {
        clear();
        printGrid(grid);
        printw("Number from 1 to 5 to change speed\n");
        updateGrid(grid);
        refresh();
        char v = getch();
        if (v >= '1' && v <= '5') {
            time = setTime(v);
        }
        usleep(time);
        checkStopLife(grid, &flagToEnd, &detected_no_life);
    }
    endwin();
    if (detected_no_life) {
        printf("detected_no_life");
    };
}
void inputGrid(int grid[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            scanf("%d", &grid[i][j]);
        }
    }
}

void printGrid(int grid[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) printw("%d", grid[i][j]);
        printw("\n");
    }
    printw("\n");
}

void updateGrid(int grid[ROWS][COLS]) {
    int newGrid[ROWS][COLS];
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int neighborsCount = countNeighbors(i, j, grid);
            if (grid[i][j] == 1) {
                newGrid[i][j] = (neighborsCount == 2 || neighborsCount == 3) ? 1 : 0;
            } else {
                newGrid[i][j] = (neighborsCount == 3) ? 1 : 0;
            }
        }
    }
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            grid[i][j] = newGrid[i][j];
        }
    }
}

int countNeighbors(int row, int col, int grid[ROWS][COLS]) {
    int life_count = 0;

    for (int i = row - 1; i <= row + 1; i++) {
        int i_copy = edges_rows(i);
        for (int j = col - 1; j <= col + 1; j++) {
            int j_copy = edges_cols(j);
            if (grid[i_copy][j_copy] == 1 && !(i_copy == row && j_copy == col)) {
                life_count++;
            }
        }
    }
    return life_count;
}

int edges_rows(int i) {
    int i_copy = 0;
    switch (i) {
        case -1:
            i_copy = ROWS - 1;
            break;
        case ROWS:
            i_copy = 0;
            break;
        default:
            i_copy = i;
            break;
    }
    return i_copy;
}

int edges_cols(int j) {
    int j_copy = 0;
    switch (j) {
        case -1:
            j_copy = COLS - 1;
            break;
        case COLS:
            j_copy = 0;
            break;
        default:
            j_copy = j;
            break;
    }
    return j_copy;
}

int setTime(char v) {
    int time = 0;
    switch (v) {
        case '1':
            time = 100000;
            break;
        case '2':
            time = 200000;
            break;
        case '3':
            time = 300000;
            break;
        case '4':
            time = 400000;
            break;
        case '5':
            time = 500000;
            break;
        default:
            time = 700000;
            break;
    }
    return time;
}

void checkStopLife(int grid[ROWS][COLS], int *flagToEnd, int *detected_no_life) {
    int count = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (grid[i][j] == 0) count++;
        }
    }
    if (count == (ROWS * COLS)) {
        *flagToEnd = 0;
        *detected_no_life = 1;
    }
}
