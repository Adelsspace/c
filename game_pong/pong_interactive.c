#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 25
#define PADDLE_HEIGHT 3
#define MAX_SCORE 21

void render(int left_paddle, int right_paddle, int ball_x, int ball_y, int left_score, int right_score);
void draw_field(int left_paddle, int right_paddle, int ball_x, int ball_y);
void draw_score(int left_score, int right_score);
int check_touch_paddle_ball(int ball_y, int ball_step_x, int paddle);
int check_wall_ball(int ball_y, int ball_step_y);
int check_goal_left(int ball_x);
int check_goal_right(int ball_x);
int change_ball_x(int old_x, int direction);
int change_ball_y(int old_y, int direction);
int change_coord_left_paddle(const char in, int left_paddle);
int change_coord_right_paddle(const char in, int right_paddle);
void startGame();

int main() {
    startGame();
    return 0;
}

void startGame() {
    initscr();
    noecho();
    nodelay(stdscr, 1);

    int left_paddle = HEIGHT / 2 - PADDLE_HEIGHT / 2, right_paddle = HEIGHT / 2 - PADDLE_HEIGHT / 2;
    int left_score = 0, right_score = 0;
    int ball_x = WIDTH / 2, ball_y = (HEIGHT - 1) / 2, ball_step_x = 1, ball_step_y = 1;

    while (left_score < MAX_SCORE && right_score < MAX_SCORE) {
        render(left_paddle, right_paddle, ball_x, ball_y, left_score, right_score);

        int in = getch();

        left_paddle = change_coord_left_paddle(in, left_paddle);
        right_paddle = change_coord_right_paddle(in, right_paddle);

            if (ball_x <= 3) {
                ball_step_x = check_touch_paddle_ball(ball_y, ball_step_x, left_paddle);
            } else if (ball_x >= 78) {
                ball_step_x = check_touch_paddle_ball(ball_y, ball_step_x, right_paddle);
            }
            
            ball_step_y = check_wall_ball(ball_y, ball_step_y);
            int is_goal_scored = 0;
            if (check_goal_left(ball_x)) {
                right_score++;
                is_goal_scored = 1;
            } else if (check_goal_right(ball_x)) {
                left_score++;
                is_goal_scored = 1;
            }
            if (is_goal_scored) {
                ball_x = WIDTH / 2;
                ball_y = (HEIGHT - 1) / 2;
            } else {
                ball_x = change_ball_x(ball_x, ball_step_x);
                ball_y = change_ball_y(ball_y, ball_step_y);
            }

        usleep(200000);
    }
    if (left_score >= MAX_SCORE)
        printw("Player 1 WIN\n");
    else if (right_score >= MAX_SCORE)
        printw("Player 2 WIN\n");
    
    endwin();
}

void render(int left_paddle, int right_paddle, int ball_x, int ball_y, int left_score, int right_score) {
    clear();
    draw_field(left_paddle, right_paddle, ball_x, ball_y);
    draw_score(left_score, right_score);
    refresh();
}

void draw_field(int left_paddle, int right_paddle, int ball_x, int ball_y) {
    for (int i = 0; i < WIDTH; i++) {
        printw("-");
    }
    printw("\n");
    for (int i = 0; i < HEIGHT - 2; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0 || j == WIDTH - 1) {
                printw("|");
            } else if (i >= left_paddle - 1 && i <= left_paddle + 1 && j == 1) {
                printw("|");
            } else if (i >= right_paddle - 1 && i <= right_paddle + 1 && j == WIDTH - 2) {
                printw("|");
            } else if ((j == ball_x - 1) && (i == ball_y - 1)) {
                printw("*");
            } else {
                printw(" ");
            }
        }
        printw("\n");
    }
    for (int i = 0; i < WIDTH; i++) {
        printw("-");
    }
    printw("\n");
}

void draw_score(int left_score, int right_score) {
    printw("SCORE: Player 1: %d | Player 2: %d\n", left_score, right_score);
}

int change_coord_left_paddle(const char in, int left_paddle) {
    if ((in == 'A' || in == 'a') && left_paddle > 1) {
            left_paddle--;
        } else if ((in == 'Z' || in == 'z') && left_paddle < HEIGHT - 1 - PADDLE_HEIGHT) {
            left_paddle++;
        }
    return left_paddle;
}

int change_coord_right_paddle(const char in, int right_paddle) {
    if ((in == 'K' || in == 'k') && right_paddle > 1) {
            right_paddle--;
        } else if ((in == 'M' || in == 'm') && right_paddle < HEIGHT - 1 - PADDLE_HEIGHT) {
            right_paddle++;
        }
    return right_paddle;
}

int check_touch_paddle_ball(int ball_y, int ball_step_x, int paddle) {
    int result = ball_step_x;
    if (ball_y >= paddle - 1 && ball_y <= paddle + PADDLE_HEIGHT) {
        result = ball_step_x * -1;
    }
    return result;
}

int check_wall_ball(int ball_y, int ball_step_y) {
    int result = ball_step_y;
    if (ball_y <= 1) {
        result = 1;
    } else if (ball_y >= HEIGHT - 2) {
        result = -1;
    }
    return result;
}

int check_goal_left(int ball_x) {
    int result = 0;
    if (ball_x < 2) {
        result = 1;
    }
    return result;
}

int check_goal_right(int ball_x) {
    int result = 0;
    if (ball_x >= WIDTH - 1) {
        result = 1;
    }
    return result;
}


int change_ball_x(int old_x, int direction) { return old_x + direction; }
int change_ball_y(int old_y, int direction) { return old_y + direction; }
