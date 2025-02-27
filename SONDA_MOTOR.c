#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/i2c.h"
#include "lib/ssd1306.h"
#include "lib/font.h"

#define LAMBDA_SENSOR_PIN 26  // Pino de entrada para a sonda lambda (ADC)
#define ACCEL_SENSOR_PIN 27   // Pino de entrada para o potenciômetro de aceleração (ADC)
#define PWM_PIN 15            // Pino para controle do PWM de combustível
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

// Pinos para os LEDs RGB
#define LED_GREEN_PIN 11      // LED verde (Mistura Rica)
#define LED_BLUE_PIN 12       // LED azul (Mistura Estequiométrica)
#define LED_RED_PIN 13        // LED vermelho (Mistura Pobre)

// Pino do buzzer
#define BUZZER_PIN 10

// Variável global para o valor do PWM
uint16_t pwm_value;

// Função para configurar os pinos do LED RGB
void setup_leds() {
    gpio_init(LED_GREEN_PIN);
    gpio_set_dir(LED_GREEN_PIN, GPIO_OUT);
    
    gpio_init(LED_BLUE_PIN);
    gpio_set_dir(LED_BLUE_PIN, GPIO_OUT);
    
    gpio_init(LED_RED_PIN);
    gpio_set_dir(LED_RED_PIN, GPIO_OUT);
    
    // Inicializa todos os LEDs apagados
    gpio_put(LED_GREEN_PIN, 0);
    gpio_put(LED_BLUE_PIN, 0);
    gpio_put(LED_RED_PIN, 0);
}

// Função para configurar o ADC (sonda lambda e aceleração)
void setup_adc() {
    adc_init();
    adc_gpio_init(LAMBDA_SENSOR_PIN);  // Inicializa o pino da sonda lambda
    adc_gpio_init(ACCEL_SENSOR_PIN);   // Inicializa o pino do sensor de aceleração
}

// Função para configurar o PWM para controle de combustível
void setup_pwm() {
    gpio_set_function(PWM_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(PWM_PIN);
    pwm_set_wrap(slice_num, 255);  // Define o valor máximo de PWM (ajuste conforme necessário)
    pwm_set_gpio_level(PWM_PIN, 128);  // Valor inicial do PWM (50% do ciclo de trabalho)
    pwm_set_enabled(slice_num, true); // Habilita o PWM
}

// Função para configurar o buzzer (GPIO 21)
void setup_buzzer() {
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    gpio_put(BUZZER_PIN, 0);  // Inicializa o buzzer desligado
}

// Função para ler o valor da sonda lambda
float read_lambda() {
    adc_select_input(0);  // Garantir que o canal 0 está selecionado para a sonda lambda
    uint16_t raw_value = adc_read();  // Lê o valor bruto do ADC
    float voltage = (raw_value / 4095.0) * 3.3;  // Converte para a tensão correspondente (0-3.3V)
    return voltage;
}

// Função para ler o valor da aceleração
float read_acceleration() {
    adc_select_input(1);  // Seleciona o canal de ADC para o sensor de aceleração
    uint16_t raw_value = adc_read();  // Lê o valor bruto do ADC
    float voltage = (raw_value / 4095.0) * 3.3;  // Converte para a tensão correspondente (0-3.3V)
    return voltage;
}

// Função para calcular o fator lambda
float calculate_lambda(float lambda_voltage) {
    // Mapeia a tensão da sonda lambda para o fator lambda (1 = mistura estequiométrica)
    float lambda = 1.0;  // Valor ideal para mistura estequiométrica
    if (lambda_voltage < 0.45) {
        lambda = (lambda_voltage / 0.45); // Mistura rica
    } else if (lambda_voltage > 0.55) {
        lambda = (lambda_voltage - 0.55) / 0.45 + 1.0; // Mistura pobre
    }
    return lambda;
}

// Função para ajustar o PWM com base no fator lambda e aceleração
void adjust_fuel(float lambda, float acceleration) {
    // Lógica para controle de combustível baseada no fator lambda e aceleração
    float acceleration_factor = 1.0 + (acceleration / 3.3);  // Mapeia a aceleração para um fator

    if (lambda < 1.0) {  // Mistura rica
        pwm_value = (uint16_t)(128 - (1.0 - lambda) * 128 * acceleration_factor);  // Reduz o combustível mais em aceleração alta
    } else if (lambda > 1.0) {  // Mistura pobre
        pwm_value = (uint16_t)(128 + (lambda - 1.0) * 128 * acceleration_factor);  // Aumenta o combustível mais em aceleração alta
    } else {  // Mistura ideal
        pwm_value = 128 * acceleration_factor;  // Ajusta o PWM com base na aceleração
    }

    if (pwm_value < 0) pwm_value = 0;
    if (pwm_value > 255) pwm_value = 255;

    uint slice_num = pwm_gpio_to_slice_num(PWM_PIN);
    pwm_set_gpio_level(PWM_PIN, pwm_value);
}

// Função para controlar os LEDs RGB
void control_leds(float lambda) {
    // Desliga todos os LEDs inicialmente
    gpio_put(LED_GREEN_PIN, 0);
    gpio_put(LED_BLUE_PIN, 0);
    gpio_put(LED_RED_PIN, 0);

    if (lambda < 0.98) {
        // Mistura Rica: LED verde
        gpio_put(LED_GREEN_PIN, 1);
    } else if (lambda > 1.02) {
        // Mistura Pobre: LED vermelho
        gpio_put(LED_RED_PIN, 1);
    } else {
        // Mistura Estequiométrica: LED azul
        gpio_put(LED_BLUE_PIN, 1);
    }
}

// Função para controlar o buzzer com uma frequência audível
void control_buzzer(float lambda) {
    if (lambda >= 0.98 && lambda <= 1.02) {  // Quando a mistura for ideal (fator lambda ~ 1.0)
        // Frequência de 2kHz (500us de ciclo de onda)
        for (int i = 0; i < 1000; i++) {  // Número de ciclos de oscilação
            gpio_put(BUZZER_PIN, 1);  // Liga o buzzer
            sleep_us(500);  // Tempo alto (500us = 2kHz de frequência)
            gpio_put(BUZZER_PIN, 0);  // Desliga o buzzer
            sleep_us(500);  // Tempo baixo (500us = 2kHz de frequência)
        }
    } else {
        gpio_put(BUZZER_PIN, 0);  // Desliga o buzzer quando a mistura não for ideal
    }
}

int main() {
    stdio_init_all();
    setup_adc();  // Configura o ADC para a sonda lambda e aceleração
    setup_pwm();  // Configura o PWM para controle de combustível
    setup_leds(); // Configura os LEDs RGB
    setup_buzzer(); // Configura o buzzer

    // Inicializa o display OLED
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_t ssd; // Estrutura do display OLED
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display

    ssd1306_fill(&ssd, false);  // Limpa o display
    ssd1306_send_data(&ssd);

    while (1) {
        float lambda_voltage = read_lambda();  // Lê o valor da sonda lambda
        float acceleration_voltage = read_acceleration();  // Lê o valor da aceleração

        // Calcula o fator lambda
        float lambda = calculate_lambda(lambda_voltage);

        // Ajusta o controle de combustível com base no fator lambda e aceleração
        adjust_fuel(lambda, acceleration_voltage);

        // Calcula a porcentagem de oxigênio
        float oxygen_percentage = (lambda_voltage / 3.3) * 100;  // Mapeia a tensão da sonda lambda para % de oxigênio

        // Atualiza o conteúdo do display com os dados
        ssd1306_fill(&ssd, false);  // Limpa o display

        // Exibe os valores no display OLED
        ssd1306_draw_string(&ssd, "SONDA:", 8, 6);
        char str_lambda[10];
        sprintf(str_lambda, "%.2fV", lambda_voltage);
        ssd1306_draw_string(&ssd, str_lambda, 80, 6);  // Exibe a tensão da sonda lambda

        ssd1306_draw_string(&ssd, "OXIGE:", 8, 16);
        char str_oxygen[10];
        sprintf(str_oxygen, "%.2f%%", oxygen_percentage);
        ssd1306_draw_string(&ssd, str_oxygen, 80, 16);  // Exibe a porcentagem de oxigênio

        ssd1306_draw_string(&ssd, "COMBUS:", 8, 26);
        char str_fuel[10];
        sprintf(str_fuel, "%d", pwm_value);
        ssd1306_draw_string(&ssd, str_fuel, 80, 26);  // Exibe o valor do PWM (Gasolina)

        ssd1306_draw_string(&ssd, "ACELER:", 8, 36);
        char str_accel[10];
        sprintf(str_accel, "%.2fV", acceleration_voltage);
        ssd1306_draw_string(&ssd, str_accel, 80, 36);  // Exibe a aceleração (potenciômetro)

        ssd1306_send_data(&ssd);  // Atualiza o display

        // Controla os LEDs RGB com base no fator lambda
        control_leds(lambda);

        // Controla o buzzer com base no fator lambda
        control_buzzer(lambda);

        sleep_ms(100);  // Aguarda 100 ms antes de ler novamente
    }

    return 0;
}
