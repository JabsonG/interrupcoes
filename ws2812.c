#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "ws2812.pio.h"

#define IS_RGBW false
#define NUM_PIXELS 25
#define WS2812_PIN 7
#define BTN_A 5
#define BTN_B 6
#define LED_PIN_13 13
#define TEMPO 400

// Cores dos LEDs
uint8_t led_r = 0;
uint8_t led_g = 0;
uint8_t led_b = 200;

// Número atual a ser exibido (0 a 9)
volatile int numero_atual = 0;

// Representação dos números 0 a 9 em matriz 5x5
const bool numeros[10][5][5] = {
    {{0,1,1,1,0}, {0,1,0,1,0}, {0,1,0,1,0}, {0,1,0,1,0}, {0,1,1,1,0}}, // 0
    {{0,1,1,1,0}, {0,0,1,0,0}, {0,0,1,0,0}, {0,1,1,0,0}, {0,0,1,0,0}}, // 1
    {{0,1,1,1,0}, {0,1,0,0,0}, {0,1,1,1,0}, {0,0,0,0,1}, {0,1,1,1,0}}, // 2
    {{0,1,1,1,0}, {0,0,0,0,1}, {0,1,1,1,0}, {0,0,0,0,1}, {0,1,1,1,0}}, // 3
    {{0,1,0,0,0}, {0,0,0,0,1}, {0,1,1,1,0}, {0,1,0,0,1}, {0,1,0,0,1}}, // 4
    {{0,1,1,1,0}, {0,0,0,0,1}, {0,1,1,1,0}, {0,1,0,0,0}, {0,1,1,1,0}}, // 5
    {{0,1,1,1,0}, {0,1,0,0,1}, {0,1,1,1,0}, {0,1,0,0,0}, {0,1,1,1,0}}, // 6
    {{0,1,0,0,0}, {0,0,0,0,1}, {0,1,0,0,0}, {0,0,0,0,1}, {0,1,1,1,0}}, // 7
    {{0,1,1,1,0}, {0,1,0,0,1}, {0,1,1,1,0}, {0,1,0,0,1}, {0,1,1,1,0}}, // 8
    {{0,1,1,1,0}, {0,0,0,0,1}, {0,1,1,1,0}, {0,1,0,0,1}, {0,1,1,1,0}}, // 9
};

// Buffer da matriz 5x5
bool led_buffer[NUM_PIXELS] = {0};

// Funções auxiliares para controle dos LEDs
static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

// Atualiza o buffer com base no número atual
void atualizar_led_buffer(int num) {
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
            led_buffer[y * 5 + x] = numeros[num][y][x];
        }
    }
}

// Exibe o número na matriz de LEDs
void mostrar_numero() {
    uint32_t cor = urgb_u32(led_r, led_g, led_b);
    for (int i = 0; i < NUM_PIXELS; i++) {
        put_pixel(led_buffer[i] ? cor : 0);
    }
}

// Função para piscar o LED no pino 13 5 vezes por segundo
void piscar_led_13() {
    static bool estado_led = false;
    gpio_put(LED_PIN_13, estado_led);  // Acende ou apaga o LED
    estado_led = !estado_led;  // Alterna o estado
    sleep_ms(100);  // 100ms para 5 piscadas por segundo
}

// Interrupção do botão A (incrementa número)
void botao_a_callback(uint gpio, uint32_t events) {
    static uint32_t ultima_pressao = 0;
    uint32_t agora = to_ms_since_boot(get_absolute_time());
    
    // Evita debounce com a verificação do tempo
    if (agora - ultima_pressao > 200) {  // Atraso de 200ms para debounce
        ultima_pressao = agora;
        numero_atual = (numero_atual + 1) % 10;  // Incrementa o número de 0 a 9
        atualizar_led_buffer(numero_atual);
        mostrar_numero();
    }
}

// Interrupção do botão B (também incrementa número)
void botao_b_callback(uint gpio, uint32_t events) {
    static uint32_t ultima_pressao = 0;
    uint32_t agora = to_ms_since_boot(get_absolute_time());
    
    // Evita debounce com a verificação do tempo
    if (agora - ultima_pressao > 200) {  // Atraso de 200ms para debounce
        ultima_pressao = agora;
        numero_atual = (numero_atual + 1) % 10;  // Incrementa o número de 0 a 9
        atualizar_led_buffer(numero_atual);
        mostrar_numero();
    }
}

int main() {
    // Inicializa o sistema
    stdio_init_all();
    
    // Inicializa a matriz WS2812
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
    
    // Inicializa o LED no pino 13
    gpio_init(LED_PIN_13);
    gpio_set_dir(LED_PIN_13, GPIO_OUT);
    
    // Configuração dos botões
    gpio_init(BTN_A);
    gpio_set_dir(BTN_A, GPIO_IN);
    gpio_pull_up(BTN_A);

    gpio_init(BTN_B);
    gpio_set_dir(BTN_B, GPIO_IN);
    gpio_pull_up(BTN_B);

    // Configura as interrupções para os botões
    gpio_set_irq_enabled(BTN_A, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(BTN_B, GPIO_IRQ_EDGE_FALL, true);

    // Define os callbacks para os botões
    gpio_set_irq_callback(&botao_a_callback);
    gpio_set_irq_callback(&botao_b_callback);

    // Habilita as interrupções globais
    irq_set_enabled(IO_IRQ_BANK0, true);

    // Exibe o número inicial (0)
    atualizar_led_buffer(numero_atual);
    mostrar_numero();

    while (1) {
        piscar_led_13(); // Pisca o LED 5 vezes por segundo
        sleep_ms(10);  // Evita loop infinito rodando sem necessidade
    }

    return 0;
}