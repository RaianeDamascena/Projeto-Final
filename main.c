#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "matriz_led.h"
#include "numeros.h"

#define PORTA_I2C i2c1
#define PINO_SDA 14
#define PINO_SCL 15
#define ENDERECO_DISPLAY 0x3C
#define PINO_BOTAO_A 5
#define PINO_BOTAO_B 6
#define PINO_LED_VERDE 11
#define PINO_LED_AZUL 12
#define PINO_LED_VERMELHO 13
#define TEMPO_ESPERA_BOTAO 200

volatile uint8_t indice_funcionario = 0;
volatile bool funcionario_selecionado = false;

typedef struct {
    char nome[15];
    uint8_t quantidade;
} Funcionario;

Funcionario funcionarios[3] = {
    {"Funcionario 1", 6},
    {"Funcionario 2", 5},
    {"Funcionario 3", 2}
};

// Função para desligar todos os LEDs
void resetar_leds() {
    gpio_put(PINO_LED_VERDE, 0);
    gpio_put(PINO_LED_AZUL, 0);
    gpio_put(PINO_LED_VERMELHO, 0);
}

// Função para desligar a matriz de LEDs
void resetar_matriz_led() {
    desligar_matriz();  // Chama a função para desligar todos os LEDs da matriz
}

// Atualiza o display para mostrar o funcionário selecionado
void atualizar_display(ssd1306_t *display) {
    ssd1306_fill(display, false);

    Funcionario f = funcionarios[indice_funcionario];

    // Exibe o nome do funcionário
    ssd1306_draw_string(display, f.nome, 10, 20);
    ssd1306_send_data(display);
}

// Exibe os dados do funcionário selecionado e ajusta o LED correspondente
void exibir_dados_funcionario(ssd1306_t *display) {
    Funcionario f = funcionarios[indice_funcionario];

    // Desliga todos os LEDs antes de ligar o correto
    resetar_leds();

    ssd1306_fill(display, false);
    
    char texto[30];
    sprintf(texto, "%s: %d", f.nome, f.quantidade);
    ssd1306_draw_string(display, texto, 10, 20);

    if (f.quantidade >= 6) {
        gpio_put(PINO_LED_VERDE, 1);  // Meta atingida
    } else if (f.quantidade == 5) {
        gpio_put(PINO_LED_AZUL, 1);   // Quase na meta
    } else {
        gpio_put(PINO_LED_VERMELHO, 1); // Abaixo da meta
    }

    mostrar_numero(f.quantidade);
    ssd1306_send_data(display);
}

// Callback dos botões
void funcao_botoes(uint pino, uint32_t eventos) {
    static uint32_t ultima_vez = 0;
    uint32_t tempo_atual = to_ms_since_boot(get_absolute_time());

    if ((tempo_atual - ultima_vez) < TEMPO_ESPERA_BOTAO)
        return;

    ultima_vez = tempo_atual;

    if (pino == PINO_BOTAO_A) {
        // Ao pressionar o Botão A, alterna para o próximo funcionário
        if (!funcionario_selecionado) {
            indice_funcionario = (indice_funcionario + 1) % 3; // Alterna entre os 3 funcionários
        }
    } else if (pino == PINO_BOTAO_B) {
        if (funcionario_selecionado) {
            // Se estamos no modo de exibição de dados, volta para o menu de funcionários
            funcionario_selecionado = false;
            resetar_leds();         // Desliga os LEDs ao voltar para o menu
            resetar_matriz_led();   // Desliga a matriz de LEDs ao voltar para o menu
        } else {
            // Se estamos no menu, exibe os dados do funcionário selecionado
            funcionario_selecionado = true;
        }
    }
}

int main(void) {
    stdio_init_all();
    sleep_ms(100);
    i2c_init(PORTA_I2C, 400000);
    gpio_set_function(PINO_SDA, GPIO_FUNC_I2C);
    gpio_set_function(PINO_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(PINO_SDA);
    gpio_pull_up(PINO_SCL);

    ssd1306_t display;
    ssd1306_init(&display, 128, 64, false, ENDERECO_DISPLAY, PORTA_I2C);
    ssd1306_config(&display);
    inicializar_matriz_led();

    gpio_init(PINO_BOTAO_A);
    gpio_set_dir(PINO_BOTAO_A, GPIO_IN);
    gpio_pull_up(PINO_BOTAO_A);
    gpio_init(PINO_BOTAO_B);
    gpio_set_dir(PINO_BOTAO_B, GPIO_IN);
    gpio_pull_up(PINO_BOTAO_B);
    gpio_set_irq_enabled_with_callback(PINO_BOTAO_A, GPIO_IRQ_EDGE_FALL, true, funcao_botoes);
    gpio_set_irq_enabled(PINO_BOTAO_B, GPIO_IRQ_EDGE_FALL, true);

    gpio_init(PINO_LED_VERDE);
    gpio_set_dir(PINO_LED_VERDE, GPIO_OUT);
    gpio_init(PINO_LED_AZUL);
    gpio_set_dir(PINO_LED_AZUL, GPIO_OUT);
    gpio_init(PINO_LED_VERMELHO);
    gpio_set_dir(PINO_LED_VERMELHO, GPIO_OUT);

    while (true) {
        if (funcionario_selecionado) {
            exibir_dados_funcionario(&display);  // Exibe os dados do funcionário selecionado
        } else {
            atualizar_display(&display);  // Exibe o nome do funcionário selecionado
        }
        sleep_ms(100);
    }

    return 0;
}
