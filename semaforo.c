#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

// Definição dos pinos dos LEDs
#define LED_VERMELHO 13
#define LED_AMARELO 12
#define LED_VERDE 11

// Estado atual do semáforo
volatile int estado_semaforo = 0; // Começa no vermelho, conforme especificação

// Função de callback do temporizador
bool temporizador_callback(struct repeating_timer *t) {
    // Desliga todos os LEDs
    gpio_put(LED_VERMELHO, 0);
    gpio_put(LED_AMARELO, 0);
    gpio_put(LED_VERDE, 0);

    // Alterna entre os estados do semáforo
    switch (estado_semaforo) {
        case 0:
            gpio_put(LED_VERMELHO, 1); // Vermelho
            break;
        case 1:
            gpio_put(LED_AMARELO, 1); // Amarelo
            break;
        case 2:
            gpio_put(LED_VERDE, 1); // Verde
            break;
    }
    
    // Avança para o próximo estado
    estado_semaforo = (estado_semaforo + 1) % 3;
    return true;
}

int main() {
    stdio_init_all();
    printf("Inicializando semáforo...\n");

    // Inicializa os LEDs como saída
    gpio_init(LED_VERMELHO);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_init(LED_AMARELO);
    gpio_set_dir(LED_AMARELO, GPIO_OUT);
    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERDE, GPIO_OUT);
    
    // Inicializa o temporizador repetitivo de 3 segundos
    struct repeating_timer timer;
    add_repeating_timer_ms(3000, temporizador_callback, NULL, &timer);
    
    // Loop principal
    while (true) {
        printf("Estado atual do semáforo: %d\n", estado_semaforo);
        printf("Semáforo rodando...\n");
        sleep_ms(1000); // Aguarda 1 segundo antes de imprimir novamente
    }
}