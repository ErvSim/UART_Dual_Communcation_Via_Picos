#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "pico/cyw43_arch.h"


// UART defines
// By default the stdout UART is `uart0`, so we will use the second one
#define UART_ID uart0
#define BAUD_RATE 115200

// Use pins 16 and 17 for UART1

#define UART_TX_PIN 16
#define UART_RX_PIN 17



int main()          //TX
{
    stdio_init_all();
    cyw43_arch_init();
    uint8_t TX[4] = {0x01,0xFF,0x1C,0xE2};
    uint8_t FakeTX[4] = {0x31,0x21,0x3C,0xE2};

    // Set up our UART
    uart_init(UART_ID, BAUD_RATE);
    // Set the TX and RX pins by using the function select on the GPIO

    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    

    while (true) 
    {
        for(uint i =0; i < 4; i++)
        {
            uart_putc(uart0,TX[i]);
        }
        sleep_ms(10000);
        for(uint i =0; i < 4; i++)
        {
            uart_putc(uart0,FakeTX[i]);
        }
        sleep_ms(10000);
    }
}
