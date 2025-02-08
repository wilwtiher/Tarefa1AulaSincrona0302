#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/uart.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "ws2812.pio.h"
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

// UART defines
// By default the stdout UART is `uart0`, so we will use the second one
#define UART_ID uart1
#define BAUD_RATE 115200

#define IS_RGBW false
#define NUM_PIXELS 25
#define WS2812_PIN 7
#define led_RED 13   // Red=13, Blue=12, Green=11
#define led_BLUE 12   // Red=13, Blue=12, Green=11
#define led_GREEN 11   // Red=13, Blue=12, Green=11
#define botao_pinA 5 // Botão A = 5, Botão B = 6 , BotãoJoy = 22
#define botao_pinB 6 // Botão A = 5, Botão B = 6 , BotãoJoy = 22
// Armazenar a cor (Entre 0 e 255 para intensidade)
#define led_r 5 // Intensidade do vermelho
#define led_g 5 // Intensidade do verde
#define led_b 5 // Intensidade do azul

// Use pins 4 and 5 for UART1
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define UART_TX_PIN 4
#define UART_RX_PIN 5



// Variáveis globais
static volatile uint32_t last_time = 0; // Armazena o tempo do último evento (em microssegundos)

bool led_buffer[1][NUM_PIXELS] = {
    {
        // 0 invertido verticalmente:
        // Nova ordem: linha4, linha3, linha2, linha1, linha0
        0, 1, 1, 1, 0, // antiga linha4
        1, 0, 0, 0, 1, // antiga linha3
        1, 0, 0, 0, 1, // antiga linha2
        1, 0, 0, 0, 1, // antiga linha1
        0, 1, 1, 1, 0  // antiga linha0
    }};

static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

void set_one_led(uint8_t r, uint8_t g, uint8_t b)
{
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);

    // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (led_buffer[0/*variavel do arrey do buffer*/][i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0); // Desliga os LEDs com zero no buffer
        }
    }
}

// Funcao da interrupcao
void gpio_irq_handler(uint gpio, uint32_t events)
{
    // Obtém o tempo atual em microssegundos
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    // Verifica se passou tempo suficiente desde o último evento
    if (current_time - last_time > 200000) // 200 ms de debouncing
    {
        last_time = current_time; // Atualiza o tempo do último evento
        if (gpio == botao_pinA)
        {
            
        }
        else
        {
            
        }
        set_one_led(led_r, led_g, led_b);
    }
}

int main()
{
    gpio_init(led_RED);                // Inicializa o pino do LED
    gpio_set_dir(led_RED, GPIO_OUT);   // Configura o pino como saída
    gpio_init(led_GREEN);                // Inicializa o pino do LED
    gpio_set_dir(led_GREEN, GPIO_OUT);   // Configura o pino como saída
    gpio_init(led_BLUE);                // Inicializa o pino do LED
    gpio_set_dir(led_BLUE, GPIO_OUT);   // Configura o pino como saída
    gpio_init(botao_pinA);             // Inicializa o botão
    gpio_set_dir(botao_pinA, GPIO_IN); // Configura o pino como entrada
    gpio_pull_up(botao_pinA);          // Habilita o pull-up interno
    gpio_init(botao_pinB);             // Inicializa o botão
    gpio_set_dir(botao_pinB, GPIO_IN); // Configura o pino como entrada
    gpio_pull_up(botao_pinB);          // Habilita o pull-up interno

    // configuracao do PIO
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);

    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

    // Configuração da interrupção com callback
    gpio_set_irq_enabled_with_callback(botao_pinA, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(botao_pinB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    // I2C Initialisation. Using it at 400Khz.
  i2c_init(I2C_PORT, 400 * 1000);

  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_pull_up(I2C_SDA); // Pull up the data line
  gpio_pull_up(I2C_SCL); // Pull up the clock line
  ssd1306_t ssd; // Inicializa a estrutura do display
  ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
  ssd1306_config(&ssd); // Configura o display
  ssd1306_send_data(&ssd); // Envia os dados para o display

  // Limpa o display. O display inicia com todos os pixels apagados.
  ssd1306_fill(&ssd, false);
  ssd1306_send_data(&ssd);

    // Set up our UART
    uart_init(UART_ID, BAUD_RATE);
    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    
    // Use some the various UART functions to send out data
    // In a default system, printf will also output via the default UART
    
    // Send out a string, with CR/LF conversions
    uart_puts(UART_ID, " Hello, UART!\n");
    
    // For more examples of UART use see https://github.com/raspberrypi/pico-examples/tree/master/uart

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
