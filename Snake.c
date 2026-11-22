#include <stdint.h>
#include "ripes_system.h"
// ==========================================
// 1. DEFINICIONES DE HARDWARE (MMIO) Y COLORES
// ==========================================
#define LED_MATRIX_BASE ((volatile uint32_t *)0xF0000000) 
#define DPAD_BASE       ((volatile uint32_t *)0xf0000dac)
#define SWITCH_BASE     ((volatile uint32_t *)0xf0000dbc)

#define COLOR_RED   0x00FF0000
#define COLOR_GREEN 0x0000FF00
#define COLOR_BLACK 0x00000000
#define MATRIX_WIDTH 32
#define GRID_SIZE 16

// ==========================================
// 2. ESTRUCTURAS Y VARIABLES GLOBALES
// ==========================================
// Coordenadas de la serpiente, dirección, posición de la manzana, semilla aleatoria
typedef struct {
    int x;
    int y;
} Point;

Point snake[GRID_SIZE * GRID_SIZE];
int snake_length = 1;
int dir = 3;
Point apple;
uint32_t seed = 12345;

int next_rand(){
    seed = seed * 1103515245 + 12345;
    return (seed >> 16) & 0x7FFF;
}


// ==========================================
// 3. FUNCIONES DE GRÁFICOS (LED MATRIX)
// ==========================================
// draw_block(x, y, color), clear_screen()
void spawn_apple() {
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
}

void draw_block(int x, int y, uint32_t color) {
    int px = x * 2;
    int py = y * 2;
    LED_MATRIX_BASE[py * MATRIX_WIDTH + px] = color;
    LED_MATRIX_BASE[py * MATRIX_WIDTH + (px + 1)] = color;
    LED_MATRIX_BASE[(py + 1) * MATRIX_WIDTH + px] = color;
    LED_MATRIX_BASE[(py + 1) * MATRIX_WIDTH + (px + 1)] = color;
}

void clear_screen() {
    for(int i = 0; i < MATRIX_WIDTH * MATRIX_WIDTH; i++) {
        LED_MATRIX_BASE[i] = COLOR_BLACK;
    }
}

// ==========================================
// 4. FUNCIONES DE ENTRADA (D-PAD Y SWITCH 0)
// ==========================================
// read_dpad(), wait_for_reset()
int read_dpad(int current_dir) {
    if(DPAD_BASE[0] && current_dir != 1) return 0; // Arriba
    if(DPAD_BASE[1] && current_dir != 0) return 1; // Abajo
    if(DPAD_BASE[2] && current_dir != 3) return 2; // Izquierda
    if(DPAD_BASE[3] && current_dir != 2) return 3; //derecha
    return current_dir;
}

int is_switch0_on(){
    return (SWITCH_BASE[0] & 0x1);
}
// ==========================================
// 5. LÓGICA DEL JUEGO Y MAIN
// ==========================================
int main() {
    clear_screen();
    snake[0].x = 5;
    snake[0].y = 5;

    while (1) {
        draw_block(snake[0].x, snake[0].y, COLOR_BLACK);
        dir = read_dpad(dir);
        if(dir == 0) snake[0].y -= 1;
        else if(dir == 1) snake[0].y += 1; 
        else if(dir == 2) snake[0].x -= 1; 
        else if(dir == 3) snake[0].x += 1; 
        draw_block(snake[0].x, snake[0].y, COLOR_RED);
        for (volatile int i = 0; i < 150000; i++);
    }

    return 0;
}