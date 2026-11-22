#include <stdint.h>
#include "ripes_system.h"
// ==========================================
// 1. DEFINICIONES DE HARDWARE (MMIO) Y COLORES
// ==========================================
#define LED_MATRIX_BASE ((volatile uint32_t *)0xF0000000) // Verifica esta dirección en tu pestaña I/O
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
// 3. FUNCIONES DE GRÁFICOS (LED MATRIX)
// ==========================================
// draw_block(x, y, color), clear_screen()

// ==========================================
// 4. FUNCIONES DE ENTRADA (D-PAD Y SWITCH 0)
// ==========================================
// read_dpad(), wait_for_reset()

// ==========================================
// 5. LÓGICA DEL JUEGO Y MAIN
// ==========================================
int main() {
    clear_screen();
    draw_block(5, 5, COLOR_RED); // Dibuja un bloque rojo en la posición (5,5)
    return 0;
}