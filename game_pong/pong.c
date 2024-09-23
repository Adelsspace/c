#include <stdio.h>

#define WIDTH 80
#define HEIGHT 25
#define PADDLE_HEIGHT 3
#define MAX_SCORE 21

void draw_field(int left_paddle, int right_paddle, int ball_x, int ball_y) {
    for (int i = 0; i < WIDTH; i++) {
        printf("-");
    }
    printf("\n");
    for (int i = 0; i < HEIGHT - 2; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0 || j == WIDTH - 1) {
                printf("|");
            } else if (i >= left_paddle - 1 && i <= left_paddle + 1 && j == 1) {
                printf("|");
            } else if (i >= right_paddle - 1 && i <= right_paddle + 1 && j == WIDTH - 2) {
                printf("|");
            } else if ((j == ball_x - 1) && (i == ball_y - 1)) {
                printf("*");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
    for (int i = 0; i < WIDTH; i++) {
        printf("-");
    }
    printf("\n");
}

void draw_score(int left_score, int right_score) {
    printf("SCORE: Player 1: %d | Player 2: %d\n", left_score, right_score);
}

int change_ball_x(int old_x, int direction) { return old_x + direction; }
int change_ball_y(int old_y, int direction) { return old_y + direction; }

int check_wall_ball(int ball_y, int ball_dir_y) {
    int result = ball_dir_y;
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

int check_touch_paddle_ball(int ball_y, int ball_dir_x, int paddle) {
    int result = ball_dir_x;
    if (ball_y >= paddle - 1 && ball_y <= paddle + PADDLE_HEIGHT) {
        result = ball_dir_x * -1;
    }
    return result;
}
void render(int left_paddle, int right_paddle, int ball_x, int ball_y, int left_score, int right_score) {
    printf("\033[2J");
    printf("\033[H");
    draw_field(left_paddle, right_paddle, ball_x, ball_y);
    draw_score(left_score, right_score);
}

int main() {
    int left_paddle = HEIGHT / 2 - PADDLE_HEIGHT / 2, right_paddle = HEIGHT / 2 - PADDLE_HEIGHT / 2;
    int left_score = 0, right_score = 0;
    int ball_x = WIDTH / 2, ball_y = (HEIGHT - 1) / 2, ball_dir_x = 1, ball_dir_y = 1;
    while (left_score < MAX_SCORE && right_score < MAX_SCORE) {
        render(left_paddle, right_paddle, ball_x, ball_y, left_score, right_score);
        char in;
        in = getchar();
        while (getchar() != '\n')
            ;
        int isInputValid = 1;
        if ((in == 'A' || in == 'a') && left_paddle > 1) {
            left_paddle--;
        } else if ((in == 'Z' || in == 'z') && left_paddle < HEIGHT - 1 - PADDLE_HEIGHT) {
            left_paddle++;
        } else if ((in == 'K' || in == 'k') && right_paddle > 1) {
            right_paddle--;
        } else if ((in == 'M' || in == 'm') && right_paddle < HEIGHT - 1 - PADDLE_HEIGHT) {
            right_paddle++;
        } else if (in != 32)
            isInputValid = 0;
        if (isInputValid) {
            if (ball_x <= 3) {
                ball_dir_x = check_touch_paddle_ball(ball_y, ball_dir_x, left_paddle);
            } else if (ball_x >= 78) {
                ball_dir_x = check_touch_paddle_ball(ball_y, ball_dir_x, right_paddle);
            }
            ball_dir_y = check_wall_ball(ball_y, ball_dir_y);
            int isGoalScored = 0;
            if (check_goal_left(ball_x)) {
                right_score++;
                isGoalScored = 1;
            } else if (check_goal_right(ball_x)) {
                left_score++;
                isGoalScored = 1;
            }
            if (isGoalScored) {
                ball_x = WIDTH / 2;
                ball_y = (HEIGHT - 1) / 2;
            } else {
                ball_x = change_ball_x(ball_x, ball_dir_x);
                ball_y = change_ball_y(ball_y, ball_dir_y);
            }
        }
    }
    if (left_score >= MAX_SCORE)
        printf("Player 1 WIN\n");
    else if (right_score >= MAX_SCORE)
        printf("Player 2 WIN\n");
    return 0;
}
