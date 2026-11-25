#include <stdint.h>
#include "ripes_system.h"

// ==========================================
// 1. DEFINICIONES DE HARDWARE (MMIO) Y COLORES
// ==========================================
#define LED_MATRIX_BASE ((volatile uint32_t *)LED_MATRIX_0_BASE)
#define DPAD_UP         (*(volatile uint32_t *)D_PAD_0_UP)
#define DPAD_DOWN       (*(volatile uint32_t *)D_PAD_0_DOWN)
#define DPAD_LEFT       (*(volatile uint32_t *)D_PAD_0_LEFT)
#define DPAD_RIGHT      (*(volatile uint32_t *)D_PAD_0_RIGHT)
#define SWITCH_BASE     ((volatile uint32_t *)SWITCHES_0_BASE)

#define COLOR_RED    0x00FF0000
#define COLOR_GREEN  0x0000FF00
#define COLOR_PURPLE 0x00800080
#define COLOR_BLACK  0x00000000
#define MATRIX_WIDTH 32
#define GRID_SIZE    16
#define GAME_SPEED   500

// ==========================================
// 2. ESTRUCTURAS Y VARIABLES GLOBALES
// ==========================================
typedef struct {
    int x;
    int y;
} Point;

Point snake[GRID_SIZE * GRID_SIZE];
int snake_length = 1;
int dir = 3;
int next_dir = 3;
int input_locked = 0;
int last_btn = 0;
Point apple;
uint32_t seed = 12345;

int next_rand() {
    seed = seed * 1103515245 + 12345;
    return (seed >> 16) & 0x7FFF;
}

// ==========================================
// 3. FUNCIONES DE GRÁFICOS (LED MATRIX)
// ==========================================
void draw_block(int x, int y, uint32_t color) {
    int px = x * 2;
    int py = y * 2;
    LED_MATRIX_BASE[py * MATRIX_WIDTH + px]             = color;
    LED_MATRIX_BASE[py * MATRIX_WIDTH + (px + 1)]       = color;
    LED_MATRIX_BASE[(py + 1) * MATRIX_WIDTH + px]       = color;
    LED_MATRIX_BASE[(py + 1) * MATRIX_WIDTH + (px + 1)] = color;
}

void clear_screen() {
    for (int i = 0; i < MATRIX_WIDTH * MATRIX_WIDTH; i++) {
        LED_MATRIX_BASE[i] = COLOR_BLACK;
    }
}

void spawn_apple() {
    if (snake_length >= GRID_SIZE * GRID_SIZE) return;

    int valid = 0;
    while (!valid) {
        apple.x = next_rand() % GRID_SIZE;
        apple.y = next_rand() % GRID_SIZE;
        valid = 1;
        for (int i = 0; i < snake_length; i++) {
            if (snake[i].x == apple.x && snake[i].y == apple.y) {
                valid = 0;
                break;
            }
        }
    }
    draw_block(apple.x, apple.y, COLOR_GREEN);
}

// ==========================================
// 4. FUNCIONES DE ENTRADA (D-PAD Y SWITCH 0)
// ==========================================
void poll_dpad() {
    int up    = DPAD_UP    ? 1 : 0;
    int down  = DPAD_DOWN  ? 1 : 0;
    int left  = DPAD_LEFT  ? 1 : 0;
    int right = DPAD_RIGHT ? 1 : 0;
    int pressed = up + down + left + right;

    if (pressed == 0) {
        last_btn = 0;
        return;
    }

    if (pressed > 1) {
        last_btn = -1;
        return;
    }

    int btn = 0;
    if (up)         btn = 1;
    else if (down)  btn = 2;
    else if (left)  btn = 3;
    else if (right) btn = 4;

    if (btn == last_btn) return;
    last_btn = btn;

    if (input_locked) return;

    if (btn == 1 && dir != 0 && dir != 1) {
        next_dir = 0;
        input_locked = 1;
    } else if (btn == 2 && dir != 1 && dir != 0) {
        next_dir = 1;
        input_locked = 1;
    } else if (btn == 3 && dir != 2 && dir != 3) {
        next_dir = 2;
        input_locked = 1;
    } else if (btn == 4 && dir != 3 && dir != 2) {
        next_dir = 3;
        input_locked = 1;
    }
}

int is_switch0_on() {
    return (SWITCH_BASE[0] & 0x1);
}

// ==========================================
// 5. LÓGICA DEL JUEGO Y MAIN
// ==========================================
int main() {
    clear_screen();
    snake[0].x = 5;
    snake[0].y = 5;
    snake_length = 1;
    dir = 3;
    next_dir = 3;
    input_locked = 0;
    last_btn = 0;
    draw_block(snake[0].x, snake[0].y, COLOR_RED);
    spawn_apple();

    while (1) {
        for (volatile int i = 0; i < GAME_SPEED; i++) {
            poll_dpad();
        }

        dir = next_dir;
        input_locked = 0;

        int next_x = snake[0].x;
        int next_y = snake[0].y;

        if (dir == 0)      next_y -= 1;
        else if (dir == 1) next_y += 1;
        else if (dir == 2) next_x -= 1;
        else if (dir == 3) next_x += 1;

        poll_dpad();

        int ate_apple = (next_x == apple.x && next_y == apple.y);

        int hit = (next_x < 0 || next_x >= GRID_SIZE || next_y < 0 || next_y >= GRID_SIZE);

        int check_limit = ate_apple ? snake_length : snake_length - 1;
        for (int i = 0; i < check_limit; i++) {
            if (snake[i].x == next_x && snake[i].y == next_y) {
                hit = 1;
                break;
            }
        }

        if (hit) {
            for (int i = 0; i < snake_length; i++) {
                draw_block(snake[i].x, snake[i].y, COLOR_PURPLE);
            }

            int sw_state = is_switch0_on();
            while (is_switch0_on() == sw_state);

            clear_screen();
            snake[0].x = 5;
            snake[0].y = 5;
            snake_length = 1;
            dir = 3;
            next_dir = 3;
            input_locked = 0;
            last_btn = 0;
            draw_block(snake[0].x, snake[0].y, COLOR_RED);
            spawn_apple();
            continue;
        }

        if (!ate_apple) {
            draw_block(snake[snake_length - 1].x, snake[snake_length - 1].y, COLOR_BLACK);
        } else {
            snake_length++;
        }

        for (int i = snake_length - 1; i > 0; i--) {
            snake[i] = snake[i - 1];
        }

        snake[0].x = next_x;
        snake[0].y = next_y;
        draw_block(snake[0].x, snake[0].y, COLOR_RED);

        poll_dpad();

        if (ate_apple) {
            if (snake_length >= GRID_SIZE * GRID_SIZE) {
                int sw_state = is_switch0_on();
                while (is_switch0_on() == sw_state);

                clear_screen();
                snake[0].x = 5;
                snake[0].y = 5;
                snake_length = 1;
                dir = 3;
                next_dir = 3;
                input_locked = 0;
                last_btn = 0;
                draw_block(snake[0].x, snake[0].y, COLOR_RED);
                spawn_apple();
            } else {
                spawn_apple();
            }
        }
    }

    return 0;
}
