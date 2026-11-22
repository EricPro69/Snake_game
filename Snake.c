#include <stdint.h>

// ==========================================
// 1. DEFINICIONES DE HARDWARE (MMIO) Y COLORES
// ==========================================
#define LED_MATRIX_BASE ((volatile uint32_t *)0xF0000000) // Verifica esta dirección en tu pestaña I/O
#define DPAD_BASE       ((volatile uint32_t *)0xf0000dac)
#define SWITCH_BASE     ((volatile uint32_t *)0xf0000dbc)

#define COLOR_RED   0x00FF0000
#define COLOR_GREEN 0x0000FF00
#define COLOR_BLACK 0x00000000

// ==========================================
// 2. ESTRUCTURAS Y VARIABLES GLOBALES
// ==========================================
// Coordenadas de la serpiente, dirección, posición de la manzana, semilla aleatoria

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
    while (1) {
        // Bucle principal
    }
    return 0;
}